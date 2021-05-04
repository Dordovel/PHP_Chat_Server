cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -g  -DDEBUG")

file(GLOB ADAPTER_SOURCE_FILES ${CMAKE_SOURCE_DIR}/adapter/src/*.cpp)

include_directories(header)
include_directories(interface)

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB SOURCE_FILES_HANDLER ${CMAKE_SOURCE_DIR}/Handlers/*.cpp)
file(GLOB SOURCE_FILES_PARSER ${CMAKE_SOURCE_DIR}/New_Parser/src/*.cpp)

add_executable(${PROJECT_NAME}
        ${ADAPTER_SOURCE_FILES}
        ${SOURCE_FILES}
        ${SOURCE_FILES_PARSER}
        ${SOURCE_FILES_HANDLER}
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
