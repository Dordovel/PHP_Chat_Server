cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -g")

add_executable(${PROJECT_NAME}
        client.cpp)

find_package(Threads)
if (THREADS_FOUND)
    message("Threads Found")
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

find_package(Boost REQUIRED COMPONENTS)
if(BOOST_FOUND)
    message("Boost Found")
    target_link_libraries(${PROJECT_NAME} ${CMAKE_BOOST_LIBS_INIT})
endif()
