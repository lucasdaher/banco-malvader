cmake_minimum_required(VERSION 3.27)
project(banco_malvader)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(banco_malvader
        main.cpp
        malvader.h
)
