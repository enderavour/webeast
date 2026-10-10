#ifndef ORM_TRAITS_IMPL
#define ORM_TRAITS_IMPL

#include <tuple>

namespace orm
{

template<typename T>
struct orm_model_base
{
    static inline auto constraints = std::tuple{};
};

template<typename T>
struct orm_model: orm_model_base<T>
{
    static inline auto constraints = std::tuple{};
};

};

#endif
