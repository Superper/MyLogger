cmake_minimum_required(VERSION 3.22)
project(loger)

set(CMAKE_CXX_STANDARD 14)

add_executable(loger main.cpp

        log/util/AsyncFileAppender.cpp
        log/util/AsyncFileAppender.h
        log/util/LogBuffer.cpp
        log/util/LogBuffer.h
        log/util/LogFile.cpp
        log/util/LogFile.h
        log/util/Thread.cpp
        log/util/Thread.h
        log/Logger.cpp
        log/Logger.h
        log/util/LogConfig.cpp
        log/util/LogConfig.h
        )
