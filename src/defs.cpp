#include "include/defs.hpp"

namespace defaults
{
    const int32_t CLIENTS_MAX_CAPACITY = 25;
    const std::filesystem::path STATIC_DIR_PATH = "../static";

    void default_404_handler(const Request<std::string>&, Response<std::string> &res)
    {
        res.set_status_code(HttpStatus::NotFound);
        res.set_body("404 Not Found");
    }

    void default_405_handler(const Request<std::string>&, Response<std::string> &res)
    {
        res.set_status_code(HttpStatus::MethodNotAllowed);
        res.set_body("405 Not Allowed");
    }  
}