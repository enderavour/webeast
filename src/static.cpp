#include "include/static.hpp"
#include "include/logger.hpp"
#include <fstream>
#include <vector>
#include <format>

namespace fs = std::filesystem;

StaticDir::StaticDir() = default;

StaticDir::StaticDir(const fs::path &sdir_path):
static_dir_path(sdir_path) 
{
    logger::info(std::format("Instantiated static directory with path: {}", sdir_path.c_str()));
}

void StaticDir::set_static_dir_path(const fs::path &sdir_path)
{
    logger::info(std::format("Instantiated static directory with path: {}", sdir_path.c_str()));
    static_dir_path = sdir_path;
}

std::optional<std::string> StaticDir::get_file(const fs::path &file_name)
{
    auto file_path = static_dir_path / file_name;

    if (!fs::exists(file_path))
        return {};

    std::ifstream static_file(file_path, std::ios::binary | std::ios::ate);
    std::vector<char> buf(static_file.tellg());
    static_file.seekg(0);
    static_file.read(buf.data(), buf.size());

    return std::string(buf.begin(), buf.end());
}