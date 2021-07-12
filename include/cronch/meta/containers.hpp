
#pragma once

#include <boost/hana.hpp>

#include <functional>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#include "cronch/concepts.hpp"
#include "cronch/meta/concepts.hpp"

namespace cronch::meta {
struct name {
    std::string_view value;

    constexpr explicit name(std::string_view v) : value{v} {}
};

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

template<typename T, typename R, typename S>
struct property_impl {
    using owning_type = T;
    using value_type = std::decay_t<S>;
    using retr_t = R;
    using get_t = R (T::*)() const;
    using set_t = void (T::*)(S);

    std::string_view name;
    get_t getter;
    set_t setter;

    constexpr property_impl(std::string_view n, get_t get = nullptr, set_t set = nullptr) : name{n}, getter{get}, setter{set}  {}

    constexpr auto operator()(const T& p) const -> R
    {
        if (!getter) {
            throw std::logic_error{"property asked for get without getter"};
        }
        return std::invoke(getter, &p);
    }
    constexpr void operator()(T& p, S v) const
    {
        if (!setter) {
            throw std::logic_error{"property told to set without setter"};
        }
        std::invoke(setter, &p, std::forward<S>(v));
    }
};
}

template<typename Func, typename T>
concept basic_accessor = requires(Func f, const T* p, T* p2)
{
    {std::invoke(f, p)};
    {std::invoke(f, p2, std::invoke(f, p))};
};


template<typename T, typename R, typename S>
constexpr auto property(std::string_view name, R(T::*get)() const = nullptr, void(T::*set)(S) = nullptr) {
    return detail::property_impl<T, R, S>{name, get, set};
}

template<typename T>
constexpr bool is_name = std::same_as<T, name>;

template<typename T>
struct is_field {
    enum { value = concepts::accessor<T> };
};

template<typename... Args>
class fields {
    using held_t = boost::hana::tuple<Args...>;

public:
    constexpr explicit fields(Args... values) : held_{std::move(values)...} {}

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
