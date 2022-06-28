//
// Created by Lin on 2022/6/23.
//

#include "LogFile.h"

#include <memory>
#include <utility>
#include <chrono>
#include <iostream>
//mmap形式写入文件
/*
class MMapFileWriter : public FileWriter {
public:
    MMapFileWriter(const std::string &basename, uint32_t mem_size) {
        mem_size_ = mem_size;
        writed_ = 0;
        if (fd_ >= 0) {
            close(fd_);
        }
        fd_ = open(basename.c_str(), O_RDWR | O_CREAT,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd_ < 0) {
            fprintf(stderr, "open new file failed,errno=%d", errno);
        } else {
            int n = ftruncate(fd_, mem_size);
            (void)n;
            buffer_ = (char *)mmap(NULL, mem_size_, PROT_READ | PROT_WRITE,
                                   MAP_SHARED, fd_, 0);
            if (buffer_ == MAP_FAILED) {
                fprintf(stderr, "mmap file failed,errno=%d", errno);
            }
        }
    }
    ~MMapFileWriter() {
        if (fd_ >= 0) {
            close(fd_);
            fd_ = -1;
        }
        if (buffer_ != MAP_FAILED) {
            munmap(buffer_, mem_size_);
        }
    }
    void append(const char *msg, int32_t len) {
        if (len > mem_size_ - writed_) {
            fprintf(stderr, "mmap memory overflow ,errno=%d", errno);
            return;
        }
        memcpy(buffer_ + writed_, msg, len);
        writed_ += len;
    }
    void flush() {
        if (buffer_ != MAP_FAILED) {
            msync(buffer_, mem_size_, MS_ASYNC);
        }
    }
    uint32_t writtenBytes() const { return writed_; }

private:
    int fd_;
    char *buffer_;
    int32_t mem_size_;
    int32_t writed_;
};
*/


class AppendFileWriter : public FileWriter {
private:
    std::ofstream *fp_;
    uint32_t writen_ = 0;
public:
    explicit AppendFileWriter(const std::string &filename) {
        fp_ = new std::ofstream;
        fp_->open(filename, std::ios::out);
    }

    ~AppendFileWriter() override {
        delete fp_;
    }

    void append(const char *msg, size_t len) override {

        fp_->write(msg, (long long) len);
        writen_ += len;
    }

    void flush() override { fp_->flush(); }

    uint32_t writtenBytes() const override { return writen_; }
};

LogFile::LogFile(std::string basename, uint32_t roll_size, uint32_t flush_interval, uint32_t check_interval,
                 int file_writer_type) :
        basename_(std::move(basename)),
        roll_size_(roll_size),
        flush_interval_(flush_interval),
        check_freq_count_(check_interval),
        count_(0),
        start_of_period_(0),
        last_roll_(0),
        last_flush_(0) {
    time_t now = 0;
    std::string filename = getLogFileName(basename_);
/*    是否使用mmap形式写入文件
    if (file_writer_type == FileWriterType::MMAPFILE) {
        file_ = std::make_shared<MMapFileWriter>(filename, roll_size_);
    } else {
    }*/
    file_ = std::make_shared<AppendFileWriter>(filename);
    file_writer_type_ = file_writer_type;
    rollFile();
}

void LogFile::append(const char *msg, size_t len) {
    file_->append(msg, len);
    static int aaaa  =0;
    if (file_->writtenBytes() > roll_size_) {       //写入数据大于文件大小限制，滚动日志，滚动方式暂设置为隔天滚动
        rollFile();
    } else {
        ++count_;
        if (count_ >= check_freq_count_) {          //写入次数大于写入次数限制，滚动日志
            count_ = 0;
            time_t now = time(nullptr);
            time_t this_period = now / kRollPerSeconds * kRollPerSeconds;       //当天开始时间点
            if (this_period != start_of_period_) {              //如果隔天，进行滚动检查
                rollFile();
            } else if (now - last_flush_ > flush_interval_) {   //如果大于刷新时间间隔，刷新
                last_flush_ = now;
                file_->flush();
            }
        }
    }
}

void LogFile::flush() {
    file_->flush();
}

bool LogFile::rollFile() {
    time_t now = 0;
    time_t start = now / kRollPerSeconds * kRollPerSeconds;

    if (now > last_roll_) {     //日过隔天，生成新日志
        last_roll_ = now;
        last_flush_ = now;
        start_of_period_ = start;
        std::string filename = getLogFileName(basename_);
        file_ = std::make_shared<AppendFileWriter>(filename);
    }
    return true;
}

std::string LogFile::getLogFileName(const std::string &basename) {
    std::string filename;
    filename.append(basename);
    char buf[128] = {0};
    time_t microSecondsSinceEpoch = time(nullptr);
    tm *tm_time = localtime(&microSecondsSinceEpoch);
    snprintf(buf, 128, "%4d/%02d/%02d",
             tm_time->tm_year + 1900,
             tm_time->tm_mon + 1,
             tm_time->tm_mday);
    filename.append(std::to_string(tm_time->tm_year + 1900));
    filename.append(std::to_string(tm_time->tm_mon + 1));
    filename.append(std::to_string(tm_time->tm_mday));
    filename.append(".log");
    return filename;
}