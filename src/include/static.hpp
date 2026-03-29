#ifndef STATIC_IMPL_H
#define STATIC_IMPL_H

#include <filesystem>
#include <string>

namespace sd
{

class static_dir
{
public:
    static_dir();
    static_dir(const std::filesystem::path &sdir_path);
    void set_static_dir_path(const std::filesystem::path &sdir_path);
    std::optional<std::string> get_file(const std::filesystem::path &file_name);
private:
    std::filesystem::path static_dir_path;
};

#define static_file(var, fname) \
    [&var](const http::request<std::string> &request, http::response_builder<std::string> &response) \
    { \
        response.set_status_code(http::http_status::Ok); \
        response.set_body(var.get_file(fname).value_or("<h2>Static dir error</h2>")); \
        response.set_header("Content-Type", "text/html"); \
    } 

};

#endif