cmake_minimum_required(VERSION 3.16)
project(server_smart_tag)

find_package(Threads REQUIRED)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

set(CMAKE_CXX_STANDARD 17)

set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
add_subdirectory(SQLiteCpp)

add_executable(literate-tag-server
        server.cpp security.cpp security.h commands.cpp commands.h entity.cpp entity.h db.cpp db.h)

target_link_libraries(literate-tag-server SQLiteCpp)
