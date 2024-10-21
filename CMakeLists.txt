cmake_minimum_required(VERSION 3.10)
project(ServerApp)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

# Include directories
include_directories(/usr/include/mysql)

# Add your source files here
add_executable(ServerApp server.cpp db.cpp)

# Link libraries
target_link_libraries(ServerApp mysqlclient pthread jsoncpp)
