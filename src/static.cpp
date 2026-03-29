#include "include/static.hpp"
#include "include/logger.hpp"
#include "include/defs.hpp"
#include <fstream>
#include <vector>
#include <format>

namespace fs = std::filesystem;

sd::static_dir::static_dir() = default;

sd::static_dir::static_dir(const fs::path &sdir_path):
static_dir_path(sdir_path) 
{
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format("Instantiated static directory with path: {}", sdir_path.string()));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format("Instantiated static directory with path: {}", sdir_path.string()));
#endif
}

void sd::static_dir::set_static_dir_path(const fs::path &sdir_path)
{
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format("Instantiated static directory with path: {}", sdir_path.string()));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format("Instantiated static directory with path: {}", sdir_path.string()));
#endif
    static_dir_path = sdir_path;
}

std::optional<std::string> sd::static_dir::get_file(const fs::path &file_name)
{
    auto file_path = static_dir_path / file_name;

    if (!fs::exists(file_path))
        return {};

    std::ifstream s_file(file_path, std::ios::binary | std::ios::ate);
    std::vector<char> buf(s_file.tellg());
    s_file.seekg(0);
    s_file.read(buf.data(), buf.size());

    return std::string(buf.begin(), buf.end());
}