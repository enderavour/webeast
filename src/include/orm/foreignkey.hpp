#ifndef FOREIGN_KEY_IMPL_H
#define FOREIGN_KEY_IMPL_H

#include <stdexcept>
#include <string_view>
#include <vector>
#include <type_traits>

namespace orm
{
    struct _orm_foreign_key
    {
        const char *referenced_table;

        std::vector<std::string_view> columns;
        std::vector<std::string_view> referenced_columns;
    };

    constexpr _orm_foreign_key foreign_key(
        const char *referenced_table,
        const std::vector<std::string_view> &columns,
        const std::vector<std::string_view> &referenced_columns
    ) {
        if (columns.size() != referenced_columns.size())
            throw std::invalid_argument("Columns and referenced columns arguments mismatch in amount");

        return _orm_foreign_key{referenced_table, columns, referenced_columns};
    }

    template<typename T>
    constexpr bool is_fk_v = std::is_same_v<std::remove_cvref_t<T>, _orm_foreign_key>;
};

#endif
