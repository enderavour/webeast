#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#include <string_view>
#include <ostream>
#include <filesystem>
#include "config.hpp"
#include "defaults.hpp"

namespace logger
{
    void info(std::string_view msg);
    void warn(std::string_view msg);
    void error(std::string_view msg);
    void trace(std::string_view msg);
    void debug(std::string_view msg);

    void info(std::ostream &out, std::string_view msg);
    void warn(std::ostream &out, std::string_view msg);
    void error(std::ostream &out, std::string_view msg);
    void trace(std::ostream &out, std::string_view msg);
    void debug(std::ostream &out, std::string_view msg);

    void open_log_file(const std::filesystem::path &fname);
}

#define LOG_ERROR(mesg) do \
{ \
    conf::config_opts _copts = defaults::CONFIG.get_config_opts(); \
    if ((_copts).log == conf::logging_opts::Stdout) \
        logger::error(mesg); \
    else if ((_copts).log == conf::logging_opts::File) \
        logger::error(defaults::LOG_FILE_HANDLE, mesg); \
} while (0)

#define LOG_INFO(mesg) do \
{ \
    conf::config_opts _copts = defaults::CONFIG.get_config_opts(); \
    if ((_copts).log == conf::logging_opts::Stdout) \
        logger::info(mesg); \
    else if ((_copts).log == conf::logging_opts::File) \
        logger::info(defaults::LOG_FILE_HANDLE, mesg); \
} while (0)

#define LOG_WARN(mesg) do \
{ \
    conf::config_opts _copts = defaults::CONFIG.get_config_opts(); \
    if ((_copts).log == conf::logging_opts::Stdout) \
        logger::warn(mesg); \
    else if ((_copts).log == conf::logging_opts::File) \
        logger::warn(defaults::LOG_FILE_HANDLE, mesg); \
} while (0)

#define LOG_TRACE(mesg) do \
{ \
    conf::config_opts _copts = defaults::CONFIG.get_config_opts(); \
    if ((_copts).log == conf::logging_opts::Stdout) \
        logger::trace(mesg); \
    else if ((_copts).log == conf::logging_opts::File) \
        logger::trace(defaults::LOG_FILE_HANDLE, mesg); \
} while (0)

#define LOG_DEBUG(mesg) do \
{ \
    conf::config_opts _copts = defaults::CONFIG.get_config_opts(); \
    if ((_copts).log == conf::logging_opts::Stdout) \
        logger::debug(mesg); \
    else if ((_copts).log == conf::logging_opts::File) \
        logger::debug(defaults::LOG_FILE_HANDLE, mesg); \
} while (0)

#endif
