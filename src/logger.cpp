#include "include/logger.hpp"
#include <string>
#include <chrono>
#include <print>

std::string get_time()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm *tm = std::localtime(&time);

    char buffer[23]{0};
    std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", tm);

    return buffer;
}

void logger::info(std::string_view msg)
{
    std::println("{} [INFO] {}", get_time(), msg);
}

void logger::warn(std::string_view msg)
{
    std::println("{} [WARN] {}", get_time(), msg);
}

void logger::error(std::string_view msg)
{
    std::println("{} [ERROR] {}", get_time(), msg);   
}

void logger::trace(std::string_view msg)
{
    std::println("{} [TRACE] {}", get_time(), msg); 
}

void logger::debug(std::string_view msg)
{
    std::println("{} [DEBUG] {}", get_time(), msg); 
}