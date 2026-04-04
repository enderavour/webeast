#include "include/static.hpp"
#include "include/logger.hpp"
#include "include/defs.hpp"
#include <fstream>
#include <vector>
#include <format>

namespace fs = std::filesystem;

static conf::config_opts CONFIG_OPTS = defaults::CONFIG.get_config_opts();

sd::static_dir::static_dir() = default;

sd::static_dir::static_dir(const fs::path &sdir_path):
static_dir_path(sdir_path) 
{
    LOG_INFO(CONFIG_OPTS, std::format("Initialized static directory with path: {}", sdir_path.string()));
}

void sd::static_dir::set_static_dir_path(const fs::path &sdir_path)
{
    LOG_INFO(CONFIG_OPTS, std::format("Instantiated static directory with path: {}", sdir_path.string()));
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