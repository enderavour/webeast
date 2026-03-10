#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#include <string_view>
#include <ostream>

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
}

#endif