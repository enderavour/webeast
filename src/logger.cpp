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

// Stdout
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

// Ostream 
void logger::info(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [INFO] {}", get_time(), msg) << '\n';
}

void logger::warn(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [WARN] {}", get_time(), msg) << '\n';
}

void logger::error(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [ERROR] {}", get_time(), msg) << '\n';  
}

void logger::trace(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [TRACE] {}", get_time(), msg) << '\n';
}

void logger::debug(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [DEBUG] {}", get_time(), msg) << '\n'; 
}