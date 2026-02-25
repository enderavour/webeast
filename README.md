# webeast - Backend for future webpage, built with C++23

## ATTENTION! The project is not production ready yet. Currently it's built to minimal MVP. 

### Description
- The project is build using LLVM (Clang++), C++23 and Boost.Asio
- It supports only one client at current period, planned to be extended with thread pool and coroutines in future
- In future will support regex parsing of routing and static file serving

### TODO
- Segfaults after displaying message on client connection. Possibly buffering mismanagement in server implementation

- To download, build and run application:
```
git clone https://github.com/enderavour/webeast.git
cd webeast
mkdir build && cd build
cmake ..
make
./server
```
- Distribution, forking and contributions into project are welcomed.
