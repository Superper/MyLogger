//
// Created by Lin on 2022/6/21.
//

#ifndef PMI_BHF_LOGBUFFER_H
#define PMI_BHF_LOGBUFFER_H
#include <iostream>

class LogBuffer {
public:
    LogBuffer(size_t total = 1024 * 1024 * 10);
    ~LogBuffer();

    void clear();
    void append(const char *data, size_t len);
    const char *data() const;
    size_t length() const;
    size_t available() const;
    // for used by GDB
    void setCookie(void (*cookie)()) { cookie_ = cookie; }

private:
    char *data_;
    const size_t total_;    //buffer总大小
    size_t available_;      //buffer剩余大小
    size_t cur_;            //buffer存量
    // Must be outline function for cookies.
    static void cookieStart();
    static void cookieEnd();

    void (*cookie_)();
};


#endif //PMI_BHF_LOGBUFFER_H
