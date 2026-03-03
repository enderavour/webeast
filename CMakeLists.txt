cmake_minimum_required(VERSION 3.10)

project(Server CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(webeast
    src/http.cpp
    src/router.cpp
    src/main.cpp
    src/server.cpp
    src/tpool.cpp
    src/static.cpp
    src/defs.cpp
)

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(webeast PRIVATE -ggdb)
endif()
