//
// Created by Lin on 2022/6/21.
//

#include "LogBuffer.h"
LogBuffer::LogBuffer(size_t total) : total_(total), available_(total), cur_(0) {
    data_ = new char[total];
}

LogBuffer::~LogBuffer() {
    delete[] data_;
}

size_t LogBuffer::available() const { return available_; }

void LogBuffer::clear() {
    cur_ = 0;
    available_ = total_;
}

void LogBuffer::append(const char *data, size_t len) {
    memcpy(data_ + cur_, data, len);
    cur_ += len;
    available_ -= len;
}

const char *LogBuffer::data() const { return data_; }

size_t LogBuffer::length() const { return cur_; }
