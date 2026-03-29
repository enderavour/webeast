#include "include/defs.hpp"

namespace defaults
{
    const int32_t CLIENTS_MAX_CAPACITY = 25;
    const std::filesystem::path STATIC_DIR_PATH = "../static";
    const std::filesystem::path LOG_FILE_PATH = "../log/webeast.log";
    const std::filesystem::path DB_PATH = "../db/user.sqlite3";
    std::ofstream LOG_FILE_HANDLE;

    void default_404_handler(const http::request<std::string>&, http::response_builder<std::string> &res)
    {
        res.set_status_code(http::http_status::NotFound);
        res.set_body("404 Not Found");
    }

    void default_405_handler(const http::request<std::string>&, http::response_builder<std::string> &res)
    {
        res.set_status_code(http::http_status::MethodNotAllowed);
        res.set_body("405 Not Allowed");
    }  
}