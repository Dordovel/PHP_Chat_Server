cmake_minimum_required(VERSION 3.10)
project(adapter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g")

add_executable(${PROJECT_NAME}
        adapter.cpp)
