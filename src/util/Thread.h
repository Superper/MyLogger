//
// Created by zh x on 2022/6/23.
//

#ifndef PMI_BHF_THREAD_H
#define PMI_BHF_THREAD_H

#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

class Thread {
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(ThreadFunc, std::string name);

    ~Thread();

    void start();

    void join();

    bool started() const { return started_; }

//    pid_t tid() const { return tid_; }

    const std::string &name() const { return name_; }

    static int numCreated() { return numCreated_; }

private:
    void setDefaultName();

    bool started_;
    bool joined_;
    std::shared_ptr<std::thread> thread_;
    std::thread::id tid_;       // 在线程创建时再绑定
    ThreadFunc func_; // 线程回调函数
    std::string name_;
    static std::atomic_int numCreated_;
};


#endif //PMI_BHF_THREAD_H
