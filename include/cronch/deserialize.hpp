
#pragma once

#include "cronch/concepts.hpp"

namespace cronch {

template<typename T, concepts::deserizalation_backend Backend>
void deserialize(T& into, const Backend& with)
{
    with.deserialize_to(into);
}
template<typename T, concepts::deserizalation_backend Backend>
auto deserialize(const Backend& with) -> T
{
    T into;
    deserialize(into, with);
    return into;
}

} // namespace cronch
