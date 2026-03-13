#ifndef COLUMN_IMPL_H
#define COLUMN_IMPL_H

namespace orm
{

template<typename Class, typename Field>
struct Column
{
    const char *name;
    Field Class::*member;
};

template<typename Class, typename Field>
constexpr Column<Class, Field> column(const char *name, Field Class::*member)
{
    return Column<Class, Field>{name, member};
}

};

#endif