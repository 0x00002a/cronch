
#pragma once

#include <concepts>

namespace cronch::meta::concepts {
// clang-format off
    template<typename S>
    concept setter = std::invocable<S, typename S::owning_type&, typename S::value_type>;

    template<typename S>
    concept getter = requires(S s, const typename S::owning_type& p) {
        { s(p) } -> std::same_as<typename S::value_type>;
    };

    template<typename S>
    concept accessor = setter<S> && getter<S>;

// clang-format on

} // namespace cronch::meta::concepts