#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#include <string_view>

namespace logger
{
    void info(std::string_view msg);
    void warn(std::string_view msg);
    void error(std::string_view msg);
    void trace(std::string_view msg);
    void debug(std::string_view msg);
}

#endif