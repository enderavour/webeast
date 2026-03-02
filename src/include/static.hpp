#ifndef STATIC_IMPL_H
#define STATIC_IMPL_H

#include <filesystem>
#include <string>

class StaticDir
{
public:
    StaticDir();
    StaticDir(const std::filesystem::path &sdir_path);
    void set_static_dir_path(const std::filesystem::path &sdir_path);
    std::optional<std::string> get_file(const std::filesystem::path &file_name);
private:
    std::filesystem::path static_dir_path;
};

#define STATIC(var, fname) \
    [&var](const Request<std::string> &request, Response<std::string> &response) \ 
    { \
        response.set_status_code(HttpStatus::OK); \
        response.set_body(static_dir.get_file(fname).value_or("<h2>Static dir error</h2>")); \
        response.set_header("Content-Type", "text/html"); \
    } \

#endif