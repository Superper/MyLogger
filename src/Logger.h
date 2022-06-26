//
// Created by zh x on 2022/5/29.
//

#ifndef PMI_BHF_LOGGER_H
#define PMI_BHF_LOGGER_H
#include "mutex"
#include "util/LogConfig.h"
#include "util/AsyncFileAppender.h"
/*

// LOG_INFO("%s %d", arg1, arg2)
#define LOG_INFO(logmsgFormat, ...)                       \
    do                                                    \
    {                                                     \
        Logger &logger = Logger::instance();              \
        logger.setLogLevel(INFO);                         \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#define LOG_ERROR(logmsgFormat, ...)                      \
    do                                                    \
    {                                                     \
        Logger &logger = Logger::instance();              \
        logger.setLogLevel(ERROR);                        \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#define LOG_FATAL(logmsgFormat, ...)                      \
    do                                                    \
    {                                                     \
        Logger &logger = Logger::GetInstance();              \
        logger.setLogLevel(FATAL);                        \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
        exit(-1);                                         \
    } while (0)
*/

/*#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...)                      \
    do                                                    \
    {                                                     \
        Logger &logger = Logger::GetInstance();              \
        logger.setLogLevel(DEBUG);                        \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif*/

// 输出一个日志类
class Logger {
public:


    void SetLogConfig(const LogConfig& lcf);
    static Logger* GetInstance();
    Logger(const Logger* c) = delete;
    Logger operator=(Logger) = delete;
private:
    Logger();

    static Logger* instance_;
    std::shared_ptr<AsyncFileAppender> ptr_;


    std::mutex asyncfa_mutex_;
    AsyncFileAppender* asyncfa_;
    void Info(const char* msg);
    void Error(const char* msg);
    void Fatal(const char* msg);
    void Debug(const char* msg);
    void WriteLog(LogLevel log_level, const char* file_name, const char* func_name, int32_t line_num, const char* msg);
    std::string TimeToString();
    std::string GetLogLevlToString(LogLevel level);
};


#endif //PMI_BHF_LOGGER_H
