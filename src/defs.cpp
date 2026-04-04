#include "include/defs.hpp"
#include "include/logger.hpp"

namespace defaults
{
    const int32_t CLIENTS_MAX_CAPACITY = 25;
    const std::filesystem::path STATIC_DIR_PATH = "../static";
    const std::filesystem::path LOG_FILE_PATH = "../log/webeast.log";
    const std::filesystem::path DB_PATH = "../db/user.sqlite3";
    const std::filesystem::path CONFIG_PATH = "../config/webeast.toml";
    const conf::logging_opts LOG_OPTION = conf::logging_opts::File;
    const conf::server_type SERVER_TYPE = conf::server_type::Async;

    conf::config CONFIG = conf::config(CONFIG_PATH);
    std::ofstream LOG_FILE_HANDLE;

    struct log_initializer {
        log_initializer() {
            auto opts = CONFIG.get_config_opts();
            if (opts.log == conf::logging_opts::File)
                logger::open_log_file(opts.log_file_path);
        }
    } log_init;

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