//
// Created by zh x on 2022/5/29.
//

#include <iostream>
#include <vector>
#include <cstring>
#include "Logger.h"

Logger* Logger::instance_ = new Logger();
Logger* Logger::GetInstance() {

//    static std::once_flag flag;
//    std::call_once(flag,[&](){
//        instance_ = new Logger();
//    });
    return instance_;
}

void Logger::SetLogConfig(const LogConfig &lcf) {
    asyncfa_->logcfg_ = lcf;
}

void Logger::Info(const char *msg) {
    if(msg == nullptr){
        return;
    }
    WriteLog(LogLevel::INFO,__FILE__,__FUNCTION__ ,__LINE__,msg);
}

void Logger::Error(const char *msg) {
    WriteLog(LogLevel::ERROR,__FILE__,__FUNCTION__ ,__LINE__,msg);
}

void Logger::Fatal(const char *msg) {
    WriteLog(LogLevel::FATAL,__FILE__,__FUNCTION__ ,__LINE__,msg);
}

void Logger::Debug(const char *msg) {
    WriteLog(LogLevel::DEBUG,__FILE__,__FUNCTION__ ,__LINE__,msg);
}

void Logger::WriteLog(LogLevel log_level, const char *file_name, const char *func_name, int32_t line_num, const char* msg) {
    //判断日志等级
    if (log_level < asyncfa_->logcfg_.log_level) {
        return;
    }
/*    std::string str_result;
    if (nullptr != msg) {
        size_t length = strlen(msg) + 1;  //获取格式化字符串长度
        std::vector<char> fmt_bufs(length,
                                   '\0');  //创建用于存储格式化字符串的字符数组
        int writen_n = vsnprintf(&fmt_bufs[0], fmt_bufs.size(), msg, ap);
        if (writen_n > 0) {
            str_result = &fmt_bufs[0];
        }
    }
    if (str_result.empty()) {
        return;
    }*/
    std::string data;
    data.append(TimeToString()+"-");
    data.append(GetLogLevlToString(log_level) + "-");
    data.append(file_name);
    data.append("-");
    data.append(func_name);
    data.append("-");
    data.append(std::to_string(line_num) + "-");
    data.append(msg);

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

std::string Logger::GetLogLevlToString(LogLevel level) {
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
    asyncfa_ = new AsyncFileAppender("./log/");
}
