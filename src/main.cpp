#include "include/http.hpp"
#include "include/router.hpp"
#include "include/server.hpp"
#include "include/static.hpp"
#include "include/defs.hpp"
#include <iostream>
#include <print>

int32_t main()
{
    ServerInstance server("127.0.0.1", 8080);
    StaticDir static_dir(defaults::STATIC_DIR_PATH);

    Router router;
    server.include_router(router);
    server.get("/", [&static_dir](const Request<std::string> &request, Response<std::string> &response){
        response.set_status_code(HttpStatus::OK);
        response.set_body(std::move(static_dir.get_file("index.html").value_or("<h2>Static dir error</h2>")));
        response.set_header("Content-Type", "text/html");
    });
    server.get("/pic", [&static_dir](const Request<std::string> &request, Response<std::string> &response){
        response.set_status_code(HttpStatus::OK);
        response.set_body(std::move(static_dir.get_file("pic.html").value_or("<h2>Static dir error</h2>")));
        response.set_header("Content-Type", "text/html");
    });

    std::println("Server is running at 127.0.0.1:8080");

    server.start();
}