//
// Created by zh x on 2022/6/24.
//

#include <memory>
#include <utility>
#include <vector>
#include "AsyncFileAppender.h"
#include "LogFile.h"
#include"../Logger.h"
AsyncFileAppender::AsyncFileAppender(std::string basename)
        : started_(false),
          running_(false),
          persist_period_(logcfg_.file_option.log_flush_interval),
          basename_(std::move(basename)),
          //TODO cond,conutndown
          persit_thread_(std::bind(&AsyncFileAppender::threadFunc,this),"AsyncLogging"),
          cur_buffer_( new LogBuffer(logcfg_.log_buffer_size)){
              mkdir(basename_.c_str());
              start();
}

AsyncFileAppender::~AsyncFileAppender() {
    if (started_) {
        stop();
    }
}

void AsyncFileAppender::append(const char *msg, size_t len) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (cur_buffer_->available() >= len) {
        cur_buffer_->append(msg, len);
    } else {
        //当写入数据长度大于buffers剩余长度时，将cur_buffer_中的对象放入buffers_中
        buffers_.push_back(std::move(cur_buffer_));

        //重新申请buffer
        cur_buffer_ = std::make_unique<LogBuffer>(logcfg_.log_buffer_size);
        cur_buffer_->append(msg, len);

        //生产者生产完成，通知消费者
        cv_.notify_one();
    }
}

void AsyncFileAppender::start() {
    started_ = true;
    running_ = true;
    persit_thread_.start();
// TODO
//  cv_.wait(mutex_);
}

void AsyncFileAppender::stop() {
    started_ = false;
    cv_.notify_one();
    persit_thread_.join();
}

void AsyncFileAppender::threadFunc() {
    std::unique_ptr<LogBuffer> buffer(new LogBuffer(logcfg_.log_buffer_size));     //buffer的指针
    std::vector<std::unique_ptr<LogBuffer>> persist_buffers;                                  //持久化的buffer
    persist_buffers.resize(logcfg_.log_buffer_nums);                                  //设置buffer大小
    LogFile log_file(basename_, logcfg_.file_option.log_flush_file_size,                   //初始化文件写入设置
                     logcfg_.file_option.log_flush_interval, 1024);

//TODO
//    countdown_latch_.countDown();                                                               //？

    while (running_) {                                                                       //线程中循环执行
        {
            std::unique_lock<std::mutex> lock(mutex_);                                                         //互斥锁

            //if?
            while (buffers_.empty()) {                                                               //存有日志
                cv_.wait_for(lock, std::chrono::seconds(persist_period_));
            }
            if (buffers_.empty() && cur_buffer_->length() == 0) {                                 //一条日志都没有直接continue
                continue;
            }

            buffers_.push_back(std::move(cur_buffer_));

            // reset  cur_buffer_ and buffers_
            persist_buffers.swap(buffers_);
            cur_buffer_ = std::move(buffer);
            cur_buffer_->clear();
        }

        // if log is too large, drop it
        if (persist_buffers.size() > logcfg_.log_buffer_size) {
            std::cerr << "log is too large, drop some" << std::endl;
            persist_buffers.erase(persist_buffers.begin() + 1, persist_buffers.end());
        }

        // persist log
        for (const auto &thebuffer : persist_buffers) {
            log_file.append(thebuffer->data(), thebuffer->length());
        }

        // reset buffer and persist_buffers
        buffer = std::move(persist_buffers[0]);
        buffer->clear();
        persist_buffers.clear();

        log_file.flush();

        if (!started_) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (cur_buffer_->length() == 0) {
                running_ = false;
            }
        }
    }
    log_file.flush();
}

