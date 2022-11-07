cmake_minimum_required(VERSION 3.22)
project(loger)

set(CMAKE_CXX_STANDARD 17)

add_executable(loger    WIN32
        src/main.cpp
        src/util/AsyncFileAppender.cpp
        src/util/AsyncFileAppender.h
        src/util/LogBuffer.cpp
        src/util/LogBuffer.h
        src/util/LogFile.cpp
        src/util/LogFile.h
        src/Logger.cpp
        src/Logger.h
        src/util/LogConfig.h
        )
