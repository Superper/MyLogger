//
// Created by Lin on 2022/6/23.
//

#include "LogFile.h"

#include <memory>
#include <utility>

#include <iostream>

using namespace std::chrono;
LogFile::LogFile(std::string basename, uint8_t roll_type) :
        basename_(std::move(basename)),
        roll_type_(roll_type){

    yesterday_ = std::chrono::system_clock::now();

    std::string filename = getLogFileName(basename_);
    file_ = std::make_shared<AppendFileWriter>(filename);
}

void LogFile::append(const char *msg, size_t len) {
    file_->append(msg, len);

    time_t t_now = system_clock::to_time_t(system_clock::now()); //利用chrono库自带的方法转换为ctime中的time_t, 会导致精度降低
    tm *ty = localtime(&t_now);
    time_t t_yt = system_clock::to_time_t(yesterday_);
    tm *yty = localtime(&t_yt);

    if(roll_type_ == 1){    //roll day
        if(ty->tm_yday!=yty->tm_yday&&ty->tm_year != yty->tm_year){
            std::string filename = getLogFileName(basename_);
            file_ = std::make_shared<AppendFileWriter>(filename);
            yesterday_ = system_clock::now();
        }
    }else{
        if(ty->tm_hour!= yty->tm_hour&&ty->tm_yday!=yty->tm_yday&&ty->tm_year != yty->tm_year){
            std::string filename = getLogFileName(basename_);
            file_ = std::make_shared<AppendFileWriter>(filename);
            yesterday_ = system_clock::now();
        }
    }
}

void LogFile::flush() {
    file_->flush();
}

std::string LogFile::getLogFileName(const std::string &basename) {
    system_clock::time_point nowTp = system_clock::now();
    time_t tt = system_clock::to_time_t(nowTp);
    tm *tm_t = localtime(&tt);

    std::string filename;
    filename.append(basename);
    filename+=std::to_string(tm_t->tm_year + 1900);
    filename+=std::to_string(tm_t->tm_mon + 1);
    filename+=std::to_string(tm_t->tm_mday);

    return filename;
}