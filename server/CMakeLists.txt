cmake_minimum_required(VERSION 3.16)
project(cpp_test)

find_package(Threads REQUIRED)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

set(CMAKE_CXX_STANDARD 17)

add_executable(literate-tag-server
        server.cpp request_parser.cpp)



