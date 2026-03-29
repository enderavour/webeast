#ifndef COLUMN_IMPL_H
#define COLUMN_IMPL_H

namespace orm
{

template<typename Class, typename Field>
struct _column
{
    const char *name;
    Field Class::*member;
};

template<typename Class, typename Field>
constexpr _column<Class, Field> column(const char *name, Field Class::*member)
{
    return _column<Class, Field>{name, member};
}

};

#endif