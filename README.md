# webeast - Backend for future webpage, built with C++23

## ATTENTION! The project is not production ready yet. Currently it's built to minimal MVP. 

### Description
- The project is build using LLVM (Clang++), C++23 and Boost.Asio
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

### TODO
- Reogranize and clean structure of ORM implementation
- Add JSON support

To download, build and run application:
```
git clone https://github.com/enderavour/webeast.git
cd webeast
mkdir build && cd build
cmake ..
make
./server
```
- Distribution, forking and contributions into project are welcomed.
