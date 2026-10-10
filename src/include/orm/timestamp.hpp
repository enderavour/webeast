#ifndef TIMESTAMP_IMPL_H
#define TIMESTAMP_IMPL_H


#include <type_traits>
namespace orm
{
    struct _orm_timestamp
    {
        const char *name;
    };

    constexpr _orm_timestamp timestamp(const char *name)
    {
        return {name};
    }

    // Helper trait for orm type
    template<typename T>
    inline constexpr bool is_timestamp_v = std::is_same_v<std::remove_cvref_t<T>, _orm_timestamp>;
};

#endif
