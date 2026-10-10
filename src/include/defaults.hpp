#ifndef DEFS_H_IMPL
#define DEFS_H_IMPL

#include <cstdint>
#include "http.hpp"
#include "config.hpp"
#include <filesystem>
#include <fstream>
#include <filesystem>

namespace defaults
{
    extern const int32_t CLIENTS_MAX_CAPACITY;
    extern const std::filesystem::path STATIC_DIR_PATH;
    extern const std::filesystem::path LOG_FILE_PATH;
    extern const std::filesystem::path DB_PATH;
    extern const std::filesystem::path CONFIG_PATH;
    extern const conf::logging_opts LOG_OPTION;
    extern const conf::server_type SERVER_TYPE;
    extern conf::config CONFIG;
    extern std::ofstream LOG_FILE_HANDLE;

    void default_404_handler(const http::request<std::string>&, http::response_builder<std::string> &res);
    void default_405_handler(const http::request<std::string>&, http::response_builder<std::string> &res);
}

#endif