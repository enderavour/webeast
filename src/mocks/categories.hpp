#ifndef MOCK_CATEGORIES_IMPL
#define MOCK_CATEGORIES_IMPL

#include "../models/categories.hpp"
#include <vector>

std::vector<categories> generate_mock_categories()
{
    return {
        {
            .id = 0,
            .name = "Electronics",
            .description = "Electronic devices"
        },
        {
            .id = 0,
            .name = "Accessories",
            .description = "Accessories for electronic devices"
        }
    };
}


#endif
