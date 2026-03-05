#include "include/http.hpp"
#include "include/router.hpp"
#include "include/server.hpp"
#include "include/static.hpp"
#include "include/defs.hpp"
#include <iostream>
#include <print>
#include <new>
#include <format>

int32_t main()
{
    ServerInstance server("127.0.0.1", 8080);
    StaticDir static_dir(defaults::STATIC_DIR_PATH);

    Router router;
    server.include_router(router);
    server.get("/", STATIC(static_dir, "index.html"));
    server.get("/pic", STATIC(static_dir, "pic.html"));
    server.put("/", [&static_dir](const Request<std::string> &request, Response<std::string> &response) {
        std::println("Body response: {}", request.m_Body);
        
        response.set_status_code(HttpStatus::OK);
        response.set_body(request.m_Body);
        response.set_header("Content-Type", "text/plain");
    });
    server.get("/form", STATIC(static_dir, "form.html"));
    server.post("/user", [&static_dir](const Request<std::string> &request, Response<std::string> &response) {
        std::println("Body after filling the form: {}", request.m_Body);
        
        response.set_status_code(HttpStatus::OK);
        response.set_body(request.m_Body);
        response.set_header("Content-Type", "text/plain");
    });
    server.get("/greet/{}", 
        [](const Request<std::string> &request, Response<std::string> &response, boost::smatch &_match)
        {
            auto matched = dynamic_get_match(_match, 1);

            response.set_status_code(HttpStatus::OK);
            response.set_body(std::format("<h1>Hello, {}!</h1>", matched));
            response.set_header("Content-Type", "text/html");
        }
    );

    std::println("Server is running at 127.0.0.1:8080");

    server.start();
}