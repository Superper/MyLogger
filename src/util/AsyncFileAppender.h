//
// Created by zh x on 2022/6/24.
//

#ifndef PMI_BHF_ASYNCFILEAPPENDER_H
#define PMI_BHF_ASYNCFILEAPPENDER_H

#include <mutex>
#include <condition_variable>
#include "Thread.h"
#include "LogBuffer.h"
#include "LogConfig.h"
#include <vector>
class AsyncFileAppender {
public:
    explicit AsyncFileAppender(std::string basename);

    ~AsyncFileAppender();

    void append(const char *msg, size_t len);

    void start();

    void stop();

    LogConfig logcfg_;
private:

    void threadFunc();
    bool started_;
    bool running_;
    time_t persist_period_;  //logfile刷新时间
    std::string basename_;

    std::mutex mutex_;
    std::condition_variable cv_;
//    Mutex mutex_;
//    Condition cond_;
//    CountDownLatch countdown_latch_;
    Thread persit_thread_;
    std::unique_ptr<LogBuffer> cur_buffer_;
    std::vector<std::unique_ptr<LogBuffer>> buffers_;
};


#endif //PMI_BHF_ASYNCFILEAPPENDER_H
