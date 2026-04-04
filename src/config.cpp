#include "include/config.hpp"
#include "include/defs.hpp"
#include <print>
#include <toml++/toml.hpp>

conf::config::config() = default;

conf::config::config(const std::filesystem::path &path)
{
    config_opts _copts;
    // Filling up default configuration settings
    if (!std::filesystem::exists(path))
    {
        _copts.clients_capacity = defaults::CLIENTS_MAX_CAPACITY;
        _copts.db_path = defaults::DB_PATH;
        _copts.log = defaults::LOG_OPTION;
        _copts.log_file_path = defaults::LOG_FILE_PATH;
        _copts.static_dir_path = defaults::STATIC_DIR_PATH;
        _copts.server = defaults::SERVER_TYPE;

        m_Copts = _copts;
    }
    // Otherwise, load settings from the file
    read_file(path);
}

void conf::config::read_file(const std::filesystem::path &path)
{
    conf::config_opts _copts;
    try 
    {
        toml::table tbl = toml::parse_file(path.c_str());

        _copts.clients_capacity = tbl["config"]["clients_capacity"].as_integer()->get();
        _copts.static_dir_path = tbl["config"]["static_dir_path"].as_string()->get();
        _copts.db_path = tbl["config"]["db_path"].as_string()->get();
        _copts.log_file_path = tbl["config"]["log_file"].as_string()->get();

        auto log_type = tbl["config"]["log_type"].as_string()->get();
    
        if (log_type == "stdout")
            _copts.log = conf::logging_opts::Stdout;
        else if (log_type == "file")
            _copts.log = conf::logging_opts::File;
        
        auto server_t = tbl["config"]["server_type"].as_string()->get();

        if (server_t == "sync")
            _copts.server = conf::server_type::Sync;
        else if (server_t == "async")
            _copts.server = conf::server_type::Async;
        
        m_Copts = _copts;
    }
    catch (const toml::parse_error &e)
    {
        std::println("Could not parse TOML config: {}", e.what());
        exit(-1);
    }
}

conf::config_opts conf::config::get_config_opts() const
{
    return m_Copts;
}