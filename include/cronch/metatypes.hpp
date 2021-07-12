
#pragma once 

#include <string>
#include <vector>

#include "cronch/metadata.hpp"

/*
CRONCH_NAMED_TYPE(int, "int")
CRONCH_NAMED_TYPE(long, "long")
CRONCH_NAMED_TYPE(short, "short")
CRONCH_NAMED_TYPE(char, "char")
CRONCH_NAMED_TYPE(unsigned int, "uint")
CRONCH_NAMED_TYPE(std::string, "string")*/

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
