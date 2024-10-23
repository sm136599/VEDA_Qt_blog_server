cmake_minimum_required(VERSION 3.10)
project(BlogServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

include_directories(/usr/include/mysql)

add_executable(BlogServer
    readjson.cpp
    writejson.cpp
    db.cpp
    thread.cpp
)

target_link_libraries(BlogServer
    mysqlclient
    pthread
    jsoncpp
)
