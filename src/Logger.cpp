//
// Created by zh x on 2022/5/29.
//

#include <iostream>
#include <vector>
#include <cstring>
#include "Logger.h"
Logger* Logger::instance_ = new Logger();
Logger* Logger::GetInstance() {
    return instance_;
}

void Logger::SetLogConfig(const LogConfig &lcf) {
    asyncfa_->logcfg_ = lcf;
}

void Logger::WriteLog(LogLevel log_level, const char *file_name,uint32_t line_num, const char* msg) {
    //判断日志等级
    if (log_level < asyncfa_->logcfg_.log_level) {
        return;
    }
    std::string data;
    data.append(TimeToString()+"-");
    data.append(GetLogLevelToString(log_level) + "-");
    data.append(file_name);
    data.append("-");
    data.append(std::to_string(line_num) + "-");
    data.append(msg);
    data.append("\n");
    {
        std::lock_guard<std::mutex> lock(asyncfa_mutex_);
        asyncfa_->append(data.data(), data.size());
    }
}

std::string Logger::TimeToString() {
    char buf[128] = {0};
    time_t microSecondsSinceEpoch = time(nullptr);
    tm *tm_time = localtime(&microSecondsSinceEpoch);
    snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d",
             tm_time->tm_year + 1900,
             tm_time->tm_mon + 1,
             tm_time->tm_mday,
             tm_time->tm_hour,
             tm_time->tm_min,
             tm_time->tm_sec);
    return buf;
}

std::string Logger::GetLogLevelToString(LogLevel level) {
    std::string str_level;
    switch (level) {
        case INFO:
            str_level = " INFO";
            break;
        case ERROR:
            str_level = "ERROR";
            break;
        case FATAL:
            str_level = "FATAL";
            break;
        case DEBUG:
            str_level = "DEBUG";
            break;
        default:
            str_level = "UNKNOW";
    }
    return str_level;
}

Logger::Logger() {
    asyncfa_ = new AsyncFileAppender();
}

