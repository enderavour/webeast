#include "include/http.hpp"
#include "include/router.hpp"
#include "include/server.hpp"
#include <iostream>

int32_t main()
{
    ServerInstance server("127.0.0.1", 8080);

    Router router;
    server.include_router(router);
    server.get("/", [](const Request<std::string> &request, Response<std::string> &response){
        response.set_status_code(HttpStatus::OK);
        response.set_body("Hello, World!");
        response.set_header("Content-Length", "text/plain");
    });

    std::cout << "Server is running at 127.0.0.1:8000" << std::endl;

    server.start();
}