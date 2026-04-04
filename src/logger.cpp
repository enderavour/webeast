#include "include/logger.hpp"
#include "include/defs.hpp"
#include <string>
#include <chrono>
#include <print>

using defaults::LOG_FILE_HANDLE;

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
    out << std::format("{} [INFO] {}", get_time(), msg) << std::endl;
}

void logger::warn(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [WARN] {}", get_time(), msg) << std::endl;
}

void logger::error(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [ERROR] {}", get_time(), msg) << std::endl;  
}

void logger::trace(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [TRACE] {}", get_time(), msg) << std::endl;
}

void logger::debug(std::ostream &out, std::string_view msg)
{
    out << std::format("{} [DEBUG] {}", get_time(), msg) << std::endl; 
}

void logger::open_log_file(const std::filesystem::path &fname)
{
    std::filesystem::create_directories(fname.parent_path());
    LOG_FILE_HANDLE.open(fname, std::ios_base::out);

    if (!LOG_FILE_HANDLE.is_open())
        std::println("[ERROR] Error opening log file");
}