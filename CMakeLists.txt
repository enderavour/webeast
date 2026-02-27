cmake_minimum_required(VERSION 3.10)

project(Server CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(server 
    src/http.cpp
    src/router.cpp
    src/main.cpp
    src/server.cpp
    src/tpool.cpp
)

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(server PRIVATE -ggdb)
endif()