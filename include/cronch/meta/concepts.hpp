
#pragma once

#include <concepts>
#include <string_view>

namespace cronch::meta::concepts {
// clang-format off
template<typename S>
concept setter = std::invocable<S, typename S::owning_type&, typename S::value_type> && requires(S& s){
    { s.name };
};

template<typename S>
concept getter = requires(S s, const typename S::owning_type& p) {
    { s(p) };
    { s.name };
};

template<typename S>
concept accessor = setter<S> && getter<S>;

template<typename V>
concept view = requires(const V& v) {
    v.map([]<typename T>(T&&) requires (getter<T> || setter<T>){});
};

// clang-format on

} // namespace cronch::meta::concepts
