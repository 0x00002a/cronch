
#pragma once 

#include <string>
#include <vector>

#include "cronch/meta.hpp"
#include "cronch/metadata.hpp"

CRONCH_META_TYPE(int)
CRONCH_META_TYPE(long)
CRONCH_META_TYPE(short)
CRONCH_META_TYPE(char)
CRONCH_META_TYPE(unsigned int)
CRONCH_META_TYPE(std::string)

namespace cronch {
template<typename T>
struct metadata<std::vector<T>> {
    constexpr static auto name = "vector";
};

template<typename T1, typename T2>
struct metadata<std::pair<T1, T2>> {
    constexpr static auto name = "pair";
    constexpr static auto fields = meta::fields( 
            meta::field("first", &std::pair<T1, T2>::first),
            meta::field("second", &std::pair<T1, T2>::second)
            );
};

} // namespace cronch
