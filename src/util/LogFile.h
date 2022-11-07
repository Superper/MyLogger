//
// Created by Lin on 2022/6/23.
//

#ifndef PMI_BHF_LOGFILE_H
#define PMI_BHF_LOGFILE_H


#include <string>
#include <memory>
#include <fstream>
#include <chrono>
class FileWriter {
public:
    FileWriter() = default;

    virtual ~FileWriter() = default;

    virtual void append(const char *msg, size_t len) = 0;

    virtual void flush() = 0;

    virtual uint32_t writtenBytes() const = 0;
};

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

class LogFile {
public:
    LogFile(std::string basename, uint8_t roll_type);

    ~LogFile() = default;

    void append(const char *logline, size_t len);

    void flush();
private:
    std::string basename_;
    std::shared_ptr<FileWriter> file_;
    uint8_t roll_type_;
    std::chrono::system_clock::time_point yesterday_ ;

    static std::string getLogFileName(const std::string &basename);
};



#endif //PMI_BHF_LOGFILE_H
