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

#endif