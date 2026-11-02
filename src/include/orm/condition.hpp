#ifndef ORM_CONDITION_IMPL
#define ORM_CONDITION_IMPL

#include "column_name.hpp"
#include <type_traits>
#include <utility>
#include <tuple>
#include <string>

namespace orm
{

template<typename T, typename Member, typename Value>
struct set_field_t
{
    Member T::* member;
    Value value;
};

template<typename T, typename Member, typename Value>
auto set_field(Member T::* member, Value &&value)
{
    return set_field_t<T, Member, std::decay_t<Value>>{
        member,
        std::forward<Value>(value)
    };
}

template<typename ...Args>
auto set(Args&& ...args)
{
    return std::make_tuple(std::forward<Args>(args)...);
}

template<typename T, typename Member, typename Value>
struct condition
{
    Member T::* member;
    Value value;
};

template<typename T, typename Member, typename Value>
auto where(Member T::* member, Value &&value)
{
    return condition<T, Member, std::decay_t<Value>>{
        member,
        std::forward<Value>(value)
    };
}

template<typename T, typename Member, typename Value>
std::string where_clause(
    const condition<T, Member, Value> &cond
)
{
    std::string sql = " WHERE ";
    sql += orm::column_name(cond.member);
    sql += " = ?";

    return sql;
}

};

#endif
