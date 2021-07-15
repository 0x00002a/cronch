
#pragma once

#include "cronch/concepts.hpp"

namespace cronch {

template<typename T, concepts::backend Backend>
auto deserialize(T& into, const Backend& with)
{
    with.parse_into(into);
}
template<typename T, concepts::backend Backend>
auto deserialize(const Backend& with) -> T
{
    T into;
    deserialize(into, with);
    return into;
}

} // namespace cronch
