# webeast - Backend for future webpage, built with C++23

## ATTENTION! The project is not production ready yet. Currently it's built to minimal MVP. 

### Description
- (The project was built using LLVM (Clang++), C++23 and Boost.Asio)
- Currently the project is built using MSVC on Windows 10.
- It supports only one client at current period, planned to be extended with thread pool and coroutines in future
- In future will support regex parsing of routing and static file serving
- Added thread pool support (25 clients maximum)
- Added static file serving, static HTML pages
- Fixed body parsing on PUT, POST requests
- Added PUT, DELETE requests
- Added keep-alive support 
- Added support of dynamic path arguments
- Fixed errors
- Added logging. Implemented writing either to stdout or to log file by defining corresponding macros: LOGGING_ENABLED_STDOUT or LOGGING_ENABLED_FILE
- Added minimal URL parsing
- Added minimal ORM on top of SQLite3 DBMS. It is still messy
- The ORM was moved fully to headers, in order to avoid multiple template instantiationss
- Added static JSON support (nlohmann::json)
- Added dynamic JSON support (nlohmann::json)
- Added partial async handling of connection, using coroutines with Boost.Asio. They are not supported fully yet. Available to compile by defining corresponding macros: WEBEAST_SERVER_ASYNC. 
- The default, synchronous thread pool version was moved under definition of WEBEAST_SERVER_SYNC, which is defined by default.

### TODO
- Cleanup the code
- Fix JSON endpoint resolving under asynchronous server architecture

To download, build and run application under *nix:
```
git clone https://github.com/enderavour/webeast.git
cd webeast
mkdir build && cd build
cmake ..
make
./server
```
To download, build and run application under Windows:
1. Download and bootstrap vcpkg:
```
git clone https://github.com/microsoft/vcpkg.git && cd vcpkg && .\vcpkg-bootstrap.bat
```
2. Install Boost.Asio, Boost.Regex, Boost.URL and SQLite3:
```
.\vcpkg install boost-asio boost-regex boost-url sqlite3
```
3. Run CMake Preset:
```
cmake --preset default
```
4. Navigate into build directory and run Ninja to compile the project:
```
cd build && ninja
```
- Distribution, forking and contributions into project are welcomed.
