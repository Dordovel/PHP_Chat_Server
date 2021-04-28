cmake_minimum_required(VERSION 3.10)
project(adapter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g")

add_executable(${PROJECT_NAME}
        src/File.cpp
        src/Pipe.cpp
        src/Query.cpp
        src/Ssl.cpp
        main.cpp)
