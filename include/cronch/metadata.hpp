
#pragma once

#include <type_traits>

namespace cronch {

namespace detail {
template<typename T>
constexpr bool always_false = false;

}

template<typename T>
struct metadata : std::false_type {
};

} // namespace cronch

#define CRONCH_META_TYPE_FIELDS(type, nfields)                                                                         \
    template<>                                                                                                         \
    struct cronch::metadata<type> {                                                                                    \
        static constexpr const char* name = #type;                                                                     \
        static constexpr auto members = cronch::meta::mems nfields;                                                    \
    };

#define CRONCH_META_TYPE_NAME(type)                                                                                    \
    template<>                                                                                                         \
    struct cronch::metadata<type> {                                                                                    \
        static constexpr const char* name = #type;                                                                     \
    };

#define CRONCH_GETMACRO(_1, _2, NAME, ...) NAME
#define CRONCH_META_TYPE(...) CRONCH_GETMACRO(__VA_ARGS__, CRONCH_META_TYPE_FIELDS, CRONCH_META_TYPE_NAME)(__VA_ARGS__)
