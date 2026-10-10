#ifndef COLUMN_IMPL_H
#define COLUMN_IMPL_H

namespace orm
{

    struct primary_key_t {};

    inline constexpr primary_key_t primary_key{}; // Phantom instance of structure to pass if want to make attribute primary key

    template<typename Class, typename Field>
    struct _column
    {
        const char *name;
        Field Class::*member;
        bool primary_key = false;
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
};

#endif
