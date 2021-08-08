
#pragma once

#include <concepts>
#include <string_view>

namespace cronch::meta::concepts {

namespace detail {
template<typename S>
concept name_type = std::same_as<std::decay_t<S>, std::string_view>;

template<typename T, typename S>
concept matches_value_type = std::same_as<std::decay_t<T>, typename S::value_type>;
}

// clang-format off
template<typename S>
concept setter = std::invocable<S, typename S::owning_type&, typename S::value_type> && requires(S& s){
    { s.name } -> detail::name_type;
};

template<typename S>
concept getter = requires(S s, const typename S::owning_type& p) {
    { s(p) } -> detail::matches_value_type<S>;
    { s.name } -> detail::name_type;
};

template<typename S>
concept accessor = setter<S> && getter<S>;

template<typename V>
concept view = requires(const V& v) {
    v.map([]<typename T>(T&&) requires (getter<T> || setter<T>){});
};

// clang-format on

} // namespace cronch::meta::concepts
