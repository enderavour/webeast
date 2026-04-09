#include "include/config.hpp"
#include "include/http.hpp"
#include "include/router.hpp"
#include "include/server.hpp"
#include "include/static.hpp"
#include "include/defs.hpp"
#include "include/logger.hpp"
#include "include/orm/orm.hpp"
#include "models/user.hpp"
#include <iostream>
#include <print>
#include <new>
#include <format>

static conf::config_opts CONFIG_OPTS = defaults::CONFIG.get_config_opts();

int32_t main()
{
    sv::server server("127.0.0.1", 8080);
    sd::static_dir static_dir(defaults::STATIC_DIR_PATH);

    orm::database db(defaults::DB_PATH);
    db.create_table<User>();

    rt::router router;
    server.include_router(router);
    server.get("/", static_file(static_dir, "index.html"));
    server.get("/pic", static_file(static_dir, "pic.html"));
    server.put("/", [&static_dir](const http::request<std::string> &request, http::response_builder<std::string> &response) {
        LOG_INFO(CONFIG_OPTS, ("Body response: {}", request.m_Body));

        response.set_status_code(http::http_status::Ok);
        response.set_body(request.m_Body);
        response.set_header("Content-Type", "text/plain");
    });
    server.get("/form", static_file(static_dir, "form.html"));
    server.post("/user", [&static_dir, &db](const http::request<std::string> &request, http::response_builder<std::string> &response) {
        auto parsed = http::parse_body_params(request.m_Body);
        User user;
        for (const auto &[k, v]: parsed)
        {
            if (k == "name")
                user.name = v;

            if (k == "id")
                user.id = std::stoi(v);
        }
        db.insert(user);

        auto users = db.select<User>("WHERE id > ?", 25);

        for (const auto &u: users)
            LOG_INFO(CONFIG_OPTS, std::format("ID: {}, Name: {}", u.id, u.name));

        LOG_INFO(CONFIG_OPTS, "User was successfully added to database");

        response.set_status_code(http::http_status::Ok);
        response.set_body("User added to database");
        response.set_header("Content-Type", "text/html");
    });
    server.get("/greet/{}",
        [](const http::request<std::string> &request, http::response_builder<std::string> &response, boost::smatch &_match)
        {
            auto matched = dynamic_get_match(_match, 1);

            response.set_status_code(http::http_status::Ok);
            response.set_body(std::format("<h1>Hello, {}!</h1>", matched));
            response.set_header("Content-Type", "text/html");
        }
    );
    server.post("/jsonp",
        [](const http::request<nlohmann::json> &request, http::response_builder<nlohmann::json> &response)
        {
            LOG_INFO(CONFIG_OPTS, std::format("Received JSON from server: {}", request.m_Body.dump()));
            auto json_obj = nlohmann::json::object({
                {"status", 200},
                {"response", "Ok!"}
            });

            response.set_status_code(http::http_status::Ok);
            response.set_body(json_obj);
            response.set_header("Content-Length", std::to_string(
               json_obj.dump().size()
            ));
            response.set_header("Content-Type", "application/json");
        }
    );
    server.post("/jsond/{}",
        [](const http::request<nlohmann::json> &request, http::response_builder<nlohmann::json> &response, boost::smatch &_match)
        {
            auto json_matched = dynamic_get_match(_match, 1);

            auto json_obj = nlohmann::json::object({
                {"status", 200},
                {"response", json_matched}
            });

            LOG_INFO(CONFIG_OPTS, std::format("JSON Object: {}", json_obj.dump()));

            response.set_status_code(http::http_status::Ok);
            response.set_body(json_obj);
            response.set_header("Content-Length", std::to_string(json_obj.dump().size()));
            response.set_header("Content-Type", "application/json");
        }
    );
    server.get("/jsond/{}",
        [](const http::request<nlohmann::json> &request, http::response_builder<nlohmann::json> &response, boost::smatch &_match)
        {
            auto json_matched = dynamic_get_match(_match, 1);

            auto json_obj = nlohmann::json::object({
                {"status", 200},
                {"response", json_matched}
            });

            response.set_status_code(http::http_status::Ok);
            response.set_body(json_obj);
            response.set_header("Content-Length", std::to_string(
               json_obj.dump().size()
            ));
            response.set_header("Content-Type", "application/json");
        }
    );

    server.head("/hd",
        [](const http::request<std::string> &request, http::response_builder<std::string> &response, boost::smatch &_match)
        {
            for (const auto &hdr: request.m_Headers)
                response.set_header(hdr.first, hdr.second);
        }
    );

    server.start();
}
