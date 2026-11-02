#ifndef COLUMN_NAME_IMPL
#define COLUMN_NAME_IMPL

#include <string>
#include <tuple>
#include <type_traits>
#include <stdexcept>
#include "column.hpp"
#include "model.hpp"
#include "../logger.hpp"


namespace orm
{

template<typename T, typename Member>
std::string column_name(Member T::* member)
{
    std::string result;

    std::apply([&](const auto& ...col) {
        (
            [&] {
                using Column = std::decay_t<decltype(col)>;

                if constexpr (
                    std::is_same_v<Column, orm::_column<T, Member>>
                ) {
                    if (col.member == member) result = col.name;
                }
            }(),
            ...
        );
    }, orm::orm_model<T>::columns);

    if (result.empty())
    {
        LOG_ERROR("ORM: Could not find the specified column");
        return std::string();
    }

    return result;
}


}


#endif
