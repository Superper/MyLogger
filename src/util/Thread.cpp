//
// Created by zh x on 2022/6/23.
//

#include "Thread.h"

#include <memory>
#include <utility>

std::atomic_int Thread::numCreated_(0);

Thread::Thread(ThreadFunc func, std::string name)
        : started_(false), joined_(false),func_(std::move(func)), name_(std::move(name)) {
    setDefaultName();
}

Thread::~Thread() {
    if (started_ && !joined_) {
        thread_->detach();                                                  // thread类提供了设置分离线程的方法 线程运行后自动销毁（非阻塞）
    }
}

void Thread::start()                                                        // 一个Thread对象 记录的就是一个新线程的详细信息
{
    started_ = true;

    thread_ = std::make_shared<std::thread>([&]() {
        tid_ = thread_->get_id();                                        // 获取线程的tid值
        func_();                                                            // 开启一个新线程 专门执行该线程函数
    });
}

void Thread::join() {
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if (name_.empty()) {
        char buf[32] = {0};
        snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = buf;
    }
}