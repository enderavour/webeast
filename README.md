# webeast - Backend for future webpage, built with C++23

## ATTENTION! The project is not production ready yet. Currently it's built to minimal MVP. 

### Description
- The project was built using LLVM (Clang++), C++23 and Boost.Asio on macOS.
- Currently not built and tested for Linux.
- As well it is is built and tested using MSVC on Windows 10.
- Added thread pool support (25 clients maximum)
- Added static file serving, static HTML pages
- Added PUT, DELETE requests
- Added keep-alive support 
- Added support of dynamic path arguments
- Added logging. Implemented writing either to stdout or to log file by defining corresponding macros: LOGGING_ENABLED_STDOUT or LOGGING_ENABLED_FILE
- Added minimal URL parsing
- Added minimal ORM on top of SQLite3 DBMS. It is still messy
- The ORM was moved fully to headers, in order to avoid multiple template instantiationss
- Added static JSON support (nlohmann::json)
- Added dynamic JSON support (nlohmann::json)
- (Added partial async handling of connection, using coroutines with Boost.Asio. Available to compile by defining corresponding macros: WEBEAST_SERVER_ASYNC (Defined by default)).
- (The default, synchronous thread pool version was moved under definition of WEBEAST_SERVER_SYNC).
- Started rewriting the architecture and naming conventions of the app parts. Not compatible with previous namings and scopes
- Added configuration via TOML format. View ./config/webeast.config
- Changed logging architecture.
- Removed compilation macros (WEBEAST_SERVER_SYNC, WEBEAST_SERVER_ASYNC, LOGGING_ENABLED_STDOUT, LOGGING_ENABLED_FILE) based architecture. Now everything adjusts dynamically via webeast.toml or pulls from default settings
- Modified ORM structure, rewrote logging inside ORM 

### TODO
- Cleanup the code
- Modify the architecture of ORM
- Test config implementations
- Implement middleware

### Prerequisites:
- Boost.Asio
- Boost.Regex
- Boost.Url
- SQLite3 
- toml++

To download, build and run application under *nix:
```
git clone https://github.com/enderavour/webeast.git
cd webeast
mkdir build && cd build
cmake ..
make
./webeast
```
To download, build and run application under Windows:
1. Download and bootstrap vcpkg:
```
git clone https://github.com/microsoft/vcpkg.git && cd vcpkg && .\vcpkg-bootstrap.bat
```
2. Install Boost.Asio, Boost.Regex, Boost.URL, SQLite3 and toml++:
```
.\vcpkg install boost-asio boost-regex boost-url sqlite3 tomlplusplus
```
3. Run CMake Preset:
```
cmake --preset default
```
4. Navigate into build directory and run Ninja to compile the project:
```
cd build && ninja
```
5. Run
```
.\webeast
```
- Distribution, forking and contributions into project are welcomed.
