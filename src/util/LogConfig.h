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
    uint32_t log_buffer_size = 1000;
    uint32_t log_buffer_nums = 2;
    LogLevel log_level = LogLevel::INFO;
    struct FileOption {
        std::string file_path = "./log/";  //日志路径
        uint8_t roll_type;     //1:隔天滚动   2：小时滚动
        uint8_t log_flush_interval = 5; //刷新事件
    } file_option;
};


#endif //LOGER_LOGCONFIG_H
