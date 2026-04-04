cmake_minimum_required(VERSION 3.10)

project(webeast CXX)

set(CMAKE_CXX_STANDARD 23)

cmake_policy(SET CMP0167 NEW)

find_package(boost_regex CONFIG REQUIRED)
find_package(boost_asio CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS url)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(webeast ${SOURCES})

target_link_libraries(webeast PRIVATE Boost::regex)
target_link_libraries(webeast PRIVATE Boost::asio)
target_link_libraries(webeast PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(webeast PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(webeast PRIVATE Boost::url)

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(webeast PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_options(webeast PRIVATE /Zi)
    target_compile_options(webeast PRIVATE /EHsc)
endif()
