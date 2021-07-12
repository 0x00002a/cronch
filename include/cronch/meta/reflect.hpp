
#pragma once

#include "cronch/concepts.hpp"
#include "cronch/meta/concepts.hpp"

namespace cronch::meta {

template<cronch::concepts::has_members T>
constexpr auto accessors() -> const concepts::view auto&
{
    return metadata<T>::members;
}
template<cronch::concepts::serializable T>
constexpr auto nameof() -> const char*
{
    return metadata<T>::name;
}
} // namespace cronch::meta
