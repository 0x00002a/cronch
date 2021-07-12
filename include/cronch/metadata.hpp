
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

#define CRONCH_NAMED_TYPE(type, tname)                                          \
                                                                               \
    namespace cronch {                                                         \
    template<>                                                                 \
    struct metadata<type> {                                                    \
        static constexpr const char* name = tname;                            \
    };                                                                         \
    }
