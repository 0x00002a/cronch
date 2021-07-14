
#pragma once

#include <boost/hana/tuple.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/filter.hpp>

#include <functional>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#include "cronch/concepts.hpp"
#include "cronch/meta/concepts.hpp"

namespace cronch::meta {

template<typename T, typename Vt>
struct field {
    using field_t = Vt T::*;
    using owning_type = T;
    using value_type = Vt;

    std::string_view name;
    field_t mem_ref;

    constexpr field(std::string_view name_, field_t mem) : name{name_}, mem_ref{mem} {}

    constexpr auto operator()(const T& p) const -> const Vt& { return p.*mem_ref; }
    template<std::convertible_to<Vt> V>
    constexpr void operator()(T& p, V&& v) const
    {
        p.*mem_ref = std::forward<V>(v);
    }
};
namespace detail {
// clang-format off
template<typename T>
concept maybe_falsy = requires(T t) {
    { !t } -> std::convertible_to<bool>;
};
// clang-format on

template<typename T, typename R, typename S, typename Get, typename Set>
struct property_impl {
    using owning_type = T;
    using value_type = std::decay_t<S>;
    using retr_t = R;
    using get_t = Get;
    using set_t = Set;

    std::string_view name;
    get_t getter;
    set_t setter;

    constexpr property_impl(std::string_view n, Get get, Set set) : name{n}, getter{std::move(get)}, setter{std::move(set)}  {}

    constexpr auto operator()(const T& p) const -> R
    {
        if constexpr (maybe_falsy<Get>) {
            if (!getter) {
                throw std::logic_error{"property asked for get without getter"};
            }
        }
        return std::invoke(getter, &p);
    }
    constexpr void operator()(T& p, S v) const
    {
        if constexpr (maybe_falsy<Set>) {
            if (!setter) {
                throw std::logic_error{"property told to set without setter"};
            }
        }
        std::invoke(setter, &p, std::forward<S>(v));
    }
};
/// Alias for function pointer that acts as getter
template<typename T, typename V>
using getter_t = V(*)(const T*);

/// Alias for function pointer that acts as setter
template<typename T, typename V>
using setter_t = void(*)(T*, V);

}

template<typename Func, typename T>
concept basic_accessor = requires(Func f, const T* p, T* p2)
{
    {std::invoke(f, p)};
    {std::invoke(f, p2, std::invoke(f, p))};
};


template<typename T, typename R, typename S>
constexpr auto property(std::string_view name, R(T::*get)() const = nullptr, void(T::*set)(S) = nullptr) {
    return detail::property_impl<T, R, S, decltype(get), decltype(set)>{name, get, set};
}

template<typename T, typename R, typename S>
constexpr auto property(std::string_view name, detail::getter_t<T, R> get = nullptr, detail::setter_t<T, S> set = nullptr) {
    return detail::property_impl<T, R, S, decltype(get), decltype(set)>{name, get, set};
}
template<typename T, typename R, typename S>
constexpr auto property(std::string_view name, R(*get_base)(const T&) = nullptr, void(*set_base)(T&, S) = nullptr) {
    auto get = [get_base](const T* p) -> R { return std::invoke(get_base, *p); };
    auto set = [set_base](T* p, auto&& s) -> void { return std::invoke(set_base, *p, std::forward<decltype(s)>(s)); }; 
    return detail::property_impl<T, R, S, decltype(get), decltype(set)>{name, std::move(get), std::move(set)};
}

template<typename T>
struct is_field {
    enum { value = concepts::accessor<T> };
};

template<typename... Args>
class mems {
    using held_t = boost::hana::tuple<Args...>;

public:
    constexpr explicit mems(Args... values) : held_{std::move(values)...} {}

    template<typename Func>
    constexpr void map(Func&& f) const
    {
        boost::hana::for_each(boost::hana::filter(held_, []<typename T>(T&&) { return is_field<std::decay_t<T>>{}; }),
                              std::forward<Func>(f));
    }
    constexpr auto empty() const -> bool {
        return boost::hana::is_empty(held_);
    }

private:
    held_t held_;
};

} // namespace cronch::meta
