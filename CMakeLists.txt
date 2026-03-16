cmake_minimum_required(VERSION 3.10)

project(Server CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(SQLite3 REQUIRED)
find_package(boost_url REQUIRED)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

add_executable(webeast ${SOURCES})

target_link_libraries(webeast PRIVATE boost_url)
target_link_libraries(webeast PRIVATE SQLite::SQLite3)

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(webeast PRIVATE LOGGING_ENABLED_FILE)
    target_compile_options(webeast PRIVATE -ggdb)
endif()
