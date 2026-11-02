#ifndef COLUMN_IMPL_H
#define COLUMN_IMPL_H

#include <type_traits>

namespace orm
{

    struct primary_key_t {};
    struct autoincrement_t {};

    inline constexpr primary_key_t primary_key{}; // Phantom instance of structure to pass if want to make attribute primary key
    inline constexpr autoincrement_t autoincrement{}; // The same for autoincrement constraint

    template<typename Class, typename Field>
    struct _column
    {
        const char *name;
        Field Class::*member;
        bool primary_key = false;
        bool autoincrement = false;
    };

    template<typename Class, typename Field>
    constexpr _column<Class, Field> column(const char *name, Field Class::*member)
    {
        return _column<Class, Field>{name, member};
    }

    // Overload of the column function with primary key option
    template<typename Class, typename Field>
    constexpr _column<Class, Field> column(
        const char *name,
        Field Class::*member,
        primary_key_t
    )
    {
        return _column<Class, Field>{name, member, true};
    }

    template<typename Class, typename Field>
    constexpr _column<Class, Field> column(
        const char *name,
        Field Class::*member,
        primary_key_t,
        autoincrement_t
    )
    {
        return _column<Class, Field>{name, member, true, true};
    }

    template<typename T>
    constexpr bool has_pk_t = []{
        if constexpr (requires { T::primary_key; })
            return T::primary_key == true;
        else
            return false;
    }();
};

#endif
