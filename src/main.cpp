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
    server.get("/", STATIC(static_dir, "index.html"));
    server.get("/pic", STATIC(static_dir, "pic.html"));
    /* Not ready yet
    server.put("/", [&static_dir](const Request<std::string> &request, Response<std::string> &response) {
        std::println("{}", request.m_Headers.find("Sec-Fetch-Dest")->second);
        response.set_body(request.m_Body);
        response.set_header("Content-Type", "text/plain");
    });
    */

    std::println("Server is running at 127.0.0.1:8080");

    server.start();
}