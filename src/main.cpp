#include "include/config.hpp"
#include "include/http.hpp"
#include "include/router.hpp"
#include "include/server.hpp"
#include "include/static.hpp"
#include "include/defaults.hpp"
#include "include/logger.hpp"
#include "include/orm/orm.hpp"
#include "models/create.hpp"
#include "models/products.hpp"
#include "services/include/product.hpp"
#include <format>

#include <chrono>

static conf::config_opts CONFIG_OPTS = defaults::CONFIG.get_config_opts();

int32_t main()
{
    sv::server server("127.0.0.1", 8080);
    //sd::static_dir static_dir(defaults::STATIC_DIR_PATH);

    orm::database db(defaults::DB_PATH);
    models::create_all_tables(db);

    product_service pserv(db);

    rt::router router;
    server.include_router(router);

    // Middleware
    server.use([](auto &req, auto &res, auto next)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        next();
        auto t2 = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        LOG_INFO(CONFIG_OPTS, std::format("Time elapsed for request: {} us", ms));
    });

    //server.get("/", static_file(static_dir, "index.html"));
    //server.get("/pic", static_file(static_dir, "pic.html"));
    server.put("/", [](const http::request<std::string> &request, http::response_builder<std::string> &response) {
        LOG_INFO(CONFIG_OPTS, std::format("Body response: {}", request.m_Body));

        response.set_status_code(http::http_status::Ok);
        response.set_body(request.m_Body);
        response.set_header("Content-Type", "text/plain");
    });
    //server.get("/form", static_file(static_dir, "form.html"));
    /*server.get("/api/products/{}",
        [&pserv](
            const http::request<nlohmann::json> &request,
            http::response_builder<nlohmann::json> &response,
            boost::smatch &match
        ) {
            auto id = std::stoi(dynamic_get_match(match, 1));

            auto product = pserv.get_by_id(id);

            if (!product)
            {
                response.set_status_code(http::http_status::NotFound);
                return;
            }

            response.set_status_code(http::http_status::Ok);
            response.set_body({
                {"id", product->id},
                {"category_id", product->category_id},
                {"name", product->name},
                {"price", product->price},
                {"stock", product->stock}
            });
            response.set_header("Content-Type", "application/json");
        }
        ); */
    server.get("/api/products",
        [&pserv](
            const http::request<nlohmann::json>& request,
            http::response_builder<nlohmann::json>& response
        )
        {
            LOG_INFO(CONFIG_OPTS, "Entered /api/products handler");

            auto products = pserv.get_all();

            nlohmann::json result = nlohmann::json::array();

            for (const auto& product : products)
            {
                result.push_back({
                    {"id", product.id},
                    {"category_id", product.category_id},
                    {"name", product.name},
                    {"price", product.price},
                    {"stock", product.stock}
                });
            }


            response.set_status_code(http::http_status::Ok);
            response.set_body(result);
            response.set_header("Content-Type", "application/json");
        }
    );

    server.get("/api/products/{}",
        [&pserv](
            const http::request<nlohmann::json>& request,
            http::response_builder<nlohmann::json>& response,
            boost::smatch _match
        )
        {
            int32_t product_id = 0;

            try
            {
                const std::string id_str = dynamic_get_match(_match, 1);
                std::size_t parsed = 0;

                product_id = std::stoi(id_str, &parsed);

                if (parsed != id_str.size() || product_id <= 0)
                    throw std::invalid_argument("Invalid product ID");
            }
            catch (const std::exception&)
            {
                response.set_status_code(http::http_status::BadRequest);
                response.set_header("Content-Type", "application/json");
                response.set_body(nlohmann::json{
                    {"error", "Invalid product ID"}
                });
                return;
            }

            std::optional<products> product = pserv.get_by_id(product_id);

            response.set_header("Content-Type", "application/json");

            if (product.has_value())
            {
                response.set_status_code(http::http_status::Ok);
                response.set_body(nlohmann::json{
                    {"id", product->id},
                    {"category_id", product->category_id},
                    {"name", product->name},
                    {"price", product->price},
                    {"stock", product->stock}
                });
            }
            else
            {
                response.set_status_code(http::http_status::NotFound);
                response.set_body(nlohmann::json{
                    {"error", "Product not found"},
                    {"id", product_id}
                });
            }
        }
    );
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
    server.post("/api/products", [&db](
        const http::request<nlohmann::json> &request, http::response_builder<nlohmann::json> &response
    ) {
        const auto &body = request.m_Body;

        response.set_header("Content-Type", "application/json");

        const auto validation = validate_product_schema(body);

        if (!validation.ok())
        {
            response.set_status_code(http::http_status::BadRequest);
            response.set_body(nlohmann::json{
                {"error", "Validation failed"},
                {"details", validation.to_json()}
            });
        }

        try
        {
            db.insert<products>({
                0, body.at("category_id").get<int32_t>(), body.at("name").get<std::string>(),
                body.at("description").get<std::string>(), body.at("price").get<int32_t>(), body.at("stock").get<int32_t>()
            });

            response.set_status_code(http::http_status::Ok);
            response.set_body(nlohmann::json::object({
                {"status", 201},
                {"details", "Product was successfully added."}
            }));
        } catch (const std::exception &e)
        {
            LOG_INFO(CONFIG_OPTS, std::format("Failed to validate and add the product: {}", e.what()));

            response.set_status_code(http::http_status::InternalServerError);
            response.set_body(nlohmann::json{"error", "Internal server error"});
        }
    });
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
