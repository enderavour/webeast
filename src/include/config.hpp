#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <optional>

namespace conf
{

enum class logging_opts
{
    Stdout,
    File
};

enum class server_type
{
    Sync,
    Async
};

struct config_opts
{
    int32_t clients_capacity;
    std::filesystem::path static_dir_path;
    std::filesystem::path log_file_path;
    std::filesystem::path db_path;
    logging_opts log;
    server_type server;
};

class config
{
public:
    config();
    config(const std::filesystem::path &path);
    void read_file(const std::filesystem::path &path);
    config_opts get_config_opts() const;
private:
    config_opts m_Copts;
};

};

#endif