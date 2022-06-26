//
// Created by Lin on 2022/6/26.
//

#ifndef LOGER_LOGCONFIG_H
#define LOGER_LOGCONFIG_H

#include <string>

// 定义日志的级别 INFO ERROR FATAL DEBUG
enum LogLevel {
    INFO,  // 普通信息
    ERROR, // 错误信息
    FATAL, // core dump信息
    DEBUG, // 调试信息
};

struct LogConfig {
    uint32_t log_buffer_size = 4000;
    uint32_t log_buffer_nums = 2;
    LogLevel log_level = LogLevel::INFO;
    struct FileOption {
        std::string file_path;  //TODO 日志路径？
        uint32_t log_flush_file_size;   //每个日志大小
        uint32_t log_flush_interval;    //刷新间隔
//        FileWriterType file_writer_type;  //写入方式,暂时只实现了文件写入，mmap方式未实现
    } file_option;
};


#endif //LOGER_LOGCONFIG_H
