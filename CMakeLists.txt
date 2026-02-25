cmake_minimum_required(VERSION 3.10)

project(Server CXX)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SRC "src/*.cpp")

add_executable(server ${SRC})

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(server PRIVATE -ggdb)
endif()