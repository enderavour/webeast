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
#include "models/categories.hpp"
#include "services/include/product.hpp"
#include "services/include/categories.hpp"
#include "mocks/categories.hpp"
#include <atomic>
#include <format>

#include <chrono>
#include <string>

static conf::config_opts CONFIG_OPTS = defaults::CONFIG.get_config_opts();

int32_t main()
{
    sv::server server("127.0.0.1", 8080);
    //sd::static_dir static_dir(defaults::STATIC_DIR_PATH);

    orm::database db(defaults::DB_PATH);
    models::create_all_tables(db);

    product_service prod_service(db);
    category_service categ_service(db);

    for (const categories &category: generate_mock_categories())
        categ_service.create(category);

    rt::router router;
    server.include_router(router);

    // Timing Middleware
    server.use([](auto &req, auto &res, auto next)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        next();
        auto t2 = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        LOG_INFO( std::format("Time elapsed for request: {} us", ms));
    });

    // Products
    server.get("/api/products",
        [&prod_service](
            const http::request<nlohmann::json>& request,
            http::response_builder<nlohmann::json>& response
        )
        {
            LOG_INFO("Entered /api/products handler");

            auto products = prod_service.get_all();

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
    server._delete("/api/products/{}", [&prod_service](
        const http::request<nlohmann::json>& request,
        http::response_builder<nlohmann::json>& response,
        boost::smatch& _match
    )
    {
        const std::string id_str = dynamic_get_match(_match, 1);
        int32_t id;

        try
        {
            size_t pos = 0;
            id = std::stoi(id_str, &pos);

            if (pos != id_str.size() || id <= 0)
            {
                throw std::invalid_argument("Invalid product ID");
            }
        }
        catch (const std::exception&)
        {
            response.set_status_code(http::http_status::BadRequest);
            response.set_header("Content-Type", "application/json");
            response.set_body(nlohmann::json{
                {"status", "error"},
                {"message", "Invalid product ID"}
            });
            return;
        }

        if (!prod_service.remove(id))
        {
            response.set_status_code(http::http_status::InternalServerError);
            response.set_header("Content-Type", "application/json");
            response.set_body(nlohmann::json{
                {"status", "error"},
                {"message", "Could not remove the product"}
            });
            return;
        }

        response.set_status_code(http::http_status::Ok);
        response.set_header("Content-Type", "application/json");
        response.set_body(nlohmann::json{
            {"status", "ok"},
            {"message", "Product was successfully deleted."}
        });
    });
    server.get("/api/products/{}",
        [&prod_service](
            const http::request<nlohmann::json>& request,
            http::response_builder<nlohmann::json>& response,
            boost::smatch &_match
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

            std::optional<products> product = prod_service.get_by_id(product_id);

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
    server.put("/api/products/{}",
        [&prod_service](
            const http::request<nlohmann::json>& request,
            http::response_builder<nlohmann::json>& response,
            boost::smatch& _match
        )
        {
            int32_t product_id = 0;

            try
            {
                const auto id_str = dynamic_get_match(_match, 1);
                size_t pos = 0;

                product_id = std::stoi(id_str, &pos);

                if (pos != id_str.size() || product_id <= 0)
                    throw std::invalid_argument("Invalid product ID");
            }
            catch (const std::exception&)
            {
                response.set_status_code(http::http_status::BadRequest);
                response.set_header("Content-Type", "application/json");
                response.set_body(nlohmann::json{
                    {"status", "error"},
                    {"message", "Invalid product ID"}
                });
                return;
            }

            try
            {
                const auto& body = request.m_Body;

                products updated_product{
                    product_id,
                    body.at("category_id").get<int32_t>(),
                    body.at("name").get<std::string>(),
                    body.at("description").get<std::string>(),
                    body.at("price").get<int32_t>(),
                    body.at("stock").get<int32_t>()
                };

                prod_service.update(product_id, updated_product);

                response.set_status_code(http::http_status::Ok);
                response.set_header("Content-Type", "application/json");
                response.set_body(nlohmann::json{
                    {"status", "ok"},
                    {"message", "Product was successfully updated."}
                });
            }
            catch (const nlohmann::json::exception&)
            {
                response.set_status_code(http::http_status::BadRequest);
                response.set_header("Content-Type", "application/json");
                response.set_body(nlohmann::json{
                    {"status", "error"},
                    {"message", "Invalid product data"}
                });
            }
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
            LOG_INFO(std::format("Failed to validate and add the product: {}", e.what()));

            response.set_status_code(http::http_status::InternalServerError);
            response.set_body(nlohmann::json{"error", "Internal server error"});
        }
    });
    // -------------------------------

    // Categories
    server.get("/api/categories", [&categ_service](
        const http::request<nlohmann::json> &request,
        http::response_builder<nlohmann::json> &response
    )
    {
        auto categories = categ_service.get_all();

        nlohmann::json resulting_array = nlohmann::json::array();

        for (const auto &category: categories)
            resulting_array.push_back({
                {"id", category.id},
                {"name", category.name},
                {"description", category.description}
            });

        response.set_status_code(http::http_status::Ok);
        response.set_body(resulting_array);
        response.set_header("Content-Type", "application/json");
    });
    server.get("/api/categories/{}", [&categ_service](
        const http::request<nlohmann::json> &request,
        http::response_builder<nlohmann::json> &response,
        boost::smatch &_smatch
    )
    {
        auto categoryid_str = dynamic_get_match(_smatch, 1);
        int32_t category_id;
        category_id = std::stoi(categoryid_str, nullptr);

        auto selected_category = categ_service.get_by_id(category_id);

        if (!selected_category)
        {
            response.set_status_code(http::http_status::NotFound);
            response.set_header("Content-Type", "application/json");
            response.set_body(nlohmann::json{
                {"status", "error"},
                {"message", "Could not find the category with provided ID."}
            });
            return;
        }

        response.set_status_code(http::http_status::Ok);
        response.set_header("Content-Type", "application/json");
        response.set_body(nlohmann::json{
            {"id", selected_category->id},
            {"name", selected_category->name},
            {"description", selected_category->description}
        });
    });
    server.post("/api/categories",
        [&categ_service](
            const http::request<nlohmann::json> &request,
            http::response_builder<nlohmann::json> &response
        )
        {
            try
            {
                auto body = request.m_Body;

                categories category{};

                category.description = body.at("name").get<std::string>();
                category.name = body.at("description").get<std::string>();

                categ_service.create(category);

                response.set_status_code(http::http_status::Ok);
                response.set_header("Content-Type", "application/json");
                response.set_body(nlohmann::json{
                    {"status", "ok"},
                    {"message", "Category created successfully"}
                });
            } catch (const nlohmann::json::exception &e)
            {
                response.set_status_code(http::http_status::BadRequest);
                response.set_header("Content-Type", "application/json");
                response.set_body(nlohmann::json{
                    {"status", "error"},
                    {"message", "Invalid category data"}
                });
            }
        }
    );

    server._delete("/api/categories/{}", [&categ_service](
       const http::request<nlohmann::json> &request,
       http::response_builder<nlohmann::json> &response,
       boost::smatch &_match
    ){

        const std::string id_str = dynamic_get_match(_match, 1);
        int32_t id;

        try
        {
            size_t pos = 0;
            id = std::stoi(id_str, &pos);
            if (pos != id_str.size() || id <= 0)
                throw std::invalid_argument("Invalid category id");

        } catch (const std::exception &e)
        {
            response.set_status_code(http::http_status::BadRequest);
            response.set_header("Content-Type", "application/json");
            response.set_body(nlohmann::json{
                {"status", "error"},
                {"message", "Invalid category ID"}
            });
            return;
        }

        if (!categ_service.remove(id))
        {
            response.set_status_code(http::http_status::InternalServerError);
            response.set_header("Content-Type", "application/json");
            response.set_body(nlohmann::json{
                {"status", "error"},
                {"message", "Could not remove the category"}
            });
            return;
        }

        response.set_status_code(http::http_status::Ok);
        response.set_header("Content-Type", "application/json");
        response.set_body(nlohmann::json{
            {"status", "ok"},
            {"message", "Category was successfully removed."}
        });
        return;
    });
    server.put("/api/categories/{}", [&categ_service](
       const http::request<nlohmann::json> &request,
       http::response_builder<nlohmann::json> &response,
       boost::smatch &_match
    ){

        try
        {
            auto json_content = request.m_Body;

            int32_t id = std::stoi(dynamic_get_match(_match, 1));

            categories category{};

            category.name = json_content["name"].get<std::string>();
            category.description = json_content["description"].get<std::string>();

            categ_service.update(id, category);

            response.set_status_code(http::http_status::Ok);
            response.set_header("Content-Type", "application/json");
            response.set_body(nlohmann::json{
                {"status", "ok"},
                {"message", "Category was successfully updated."}
            });
        } catch (const std::exception &e)
        {
            response.set_status_code(http::http_status::BadRequest);
            response.set_body(nlohmann::json{
                {"status", "error"},
                {"message", e.what()}
            });
        }
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
            LOG_INFO(std::format("Received JSON from server: {}", request.m_Body.dump()));
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

    // Misc
    server.post("/jsond/{}",
        [](const http::request<nlohmann::json> &request, http::response_builder<nlohmann::json> &response, boost::smatch &_match)
        {
            auto json_matched = dynamic_get_match(_match, 1);

            auto json_obj = nlohmann::json::object({
                {"status", 200},
                {"response", json_matched}
            });

            LOG_INFO(std::format("JSON Object: {}", json_obj.dump()));

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
