cmake_minimum_required(VERSION 3.10)

project(webeast CXX)

set(CMAKE_CXX_STANDARD 23)

cmake_policy(SET CMP0167 NEW)

find_package(boost_regex CONFIG REQUIRED)
if (WIN32)
  find_package(boost_asio CONFIG REQUIRED)
endif()
find_package(Boost REQUIRED COMPONENTS url)
find_package(nlohmann_json CONFIG REQUIRED)
if (UNIX AND NOT APPLE)
    find_package(SQLite3 REQUIRED)
else()
    find_package(sqlite3 CONFIG REQUIRED)
endif()

include_directories(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(webeast ${SOURCES})

if (WIN32)
	target_link_libraries(webeast PRIVATE Boost::regex)
	target_link_libraries(webeast PRIVATE Boost::asio)
	target_link_libraries(webeast PRIVATE nlohmann_json::nlohmann_json)
	target_link_libraries(webeast PRIVATE unofficial::sqlite3::sqlite3)
	target_link_libraries(webeast PRIVATE Boost::url)
endif()


if (APPLE)
	target_link_libraries(webeast PRIVATE Boost::headers)
	target_link_libraries(webeast PRIVATE nlohmann_json::nlohmann_json)
	target_link_libraries(webeast PRIVATE sqlite3)
	target_link_libraries(webeast PRIVATE Boost::url)
endif()

if (UNIX AND NOT APPLE)
  target_link_libraries(webeast PRIVATE Boost::regex)
	target_link_libraries(webeast PRIVATE nlohmann_json::nlohmann_json)
	target_link_libraries(webeast PRIVATE SQLite3::SQLite3)
	target_link_libraries(webeast PRIVATE Boost::url)

	target_compile_options(webeast PRIVATE -fsanitize=address -g)
	target_link_options(webeast PRIVATE -fsanitize=address)
endif()

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
if (WIN32)
    target_compile_definitions(webeast PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_options(webeast PRIVATE /Zi)
    target_compile_options(webeast PRIVATE /EHsc)
endif()

if (APPLE)
	target_compile_definitions(webeast PRIVATE BOOST_ASIO_DISABLE_IOCP)
endif()
endif()
