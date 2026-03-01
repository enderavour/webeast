#ifndef DEFS_H_IMPL
#define DEFS_H_IMPL

#include <cstdint>
#include "http.hpp"
#include <filesystem>

namespace defaults
{
    extern const int32_t CLIENTS_MAX_CAPACITY;
    extern const std::filesystem::path STATIC_DIR_PATH;

    void default_404_handler(const Request<std::string>&, Response<std::string> &res);

    void default_405_handler(const Request<std::string>&, Response<std::string> &res);  
}

#endif