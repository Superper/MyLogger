//
// Created by zh x on 2022/5/29.
//

#ifndef PMI_BHF_LOGGER_H
#define PMI_BHF_LOGGER_H
#include "mutex"
#include "util/LogConfig.h"
#include "util/AsyncFileAppender.h"

#define writelog(loglevel, filename, line_num, msg) \
    do{                \
        Logger* logger = Logger::GetInstance(); \
        logger->WriteLog(loglevel,filename,line_num,msg);               \
    }while(0)

    class Logger {
    public:
        void WriteLog(LogLevel log_level, const char *file_name, uint32_t line_num, const char *msg);

        void SetLogConfig(const LogConfig &lcf);

        static Logger *GetInstance();

        Logger(const Logger *c) = delete;

        Logger operator=(Logger) = delete;

    private:
        Logger();

        static Logger *instance_;


        std::shared_ptr<AsyncFileAppender> ptr_;
        std::mutex asyncfa_mutex_;

        AsyncFileAppender *asyncfa_;

        std::string TimeToString();

        std::string GetLogLevelToString(LogLevel level);
    };
#endif //PMI_BHF_LOGGER_H
