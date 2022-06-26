//
// Created by Lin on 2022/6/23.
//

#ifndef PMI_BHF_LOGFILE_H
#define PMI_BHF_LOGFILE_H


#include <string>
#include <memory>
#include <fstream>

class FileWriter {
public:
    FileWriter() = default;

    virtual ~FileWriter() = default;

    virtual void append(const char *msg, size_t len) = 0;

    virtual void flush() = 0;

    virtual uint32_t writtenBytes() const = 0;
};



class LogFile {
public:
    LogFile(std::string basename, uint32_t roll_size,
            uint32_t flush_interval, uint32_t check_interval,
            int file_writer_type = 1);

    ~LogFile() = default;

    void append(const char *logline, size_t len);

    void flush();

    bool rollFile();

private:
    std::string basename_;
    uint32_t roll_size_;
    uint32_t flush_interval_;
    //多久进行检查一次，因为可能日志没满，但是跨天了
    uint32_t check_freq_count_;
    uint32_t count_;
    time_t start_of_period_;
    time_t last_roll_;
    time_t last_flush_;
    std::shared_ptr<FileWriter> file_;
    uint8_t file_writer_type_;
    constexpr static int kRollPerSeconds = 60 * 60 * 24;

    static std::string getLogFileName(const std::string &basename, time_t *now);
};



#endif //PMI_BHF_LOGFILE_H
