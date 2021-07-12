
#pragma once 

#include <boost/hana.hpp>

#include <stdexcept>
#include <type_traits>
#include <string_view>

#include "cronch/meta/concepts.hpp"
#include "cronch/concepts.hpp"

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

    constexpr field(std::string_view name_, field_t mem)
        : name{name_}, mem_ref{mem}
    {
    }

    constexpr auto operator()(const T& p) const -> const Vt& {
        return p.*mem_ref;
    }
    template<std::convertible_to<Vt> V>
    constexpr void operator()(T& p, V&& v) const  {
        p.*mem_ref = std::forward<V>(v);
    }
};

template<typename T, typename R, typename S>
struct property {
    using owning_type = T;
    using value_type = S;
    using retr_t = R;
    using get_t = R(*)(const T*);
    using set_t = void(*)(T*, S);
    
    get_t getter;
    set_t setter;

    property(set_t set = nullptr, get_t get = nullptr) : setter{set}, getter{get} {}

    constexpr auto operator()(const T& p) const -> R {
        if (!getter) {
            throw std::logic_error{"property asked for get without getter"};
        }
        return getter(&p);
    }
    constexpr void operator()(T& p, S v) const {
        if (!setter) {
            throw std::logic_error{"property told to set without setter"};
        }
        setter(&p, std::forward<S>(v));
    }

};

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
    constexpr explicit fields(Args... values) : held_{std::move(values)...}
    {
    }

    template<typename Func>
    constexpr void map(Func&& f) const
    {
        boost::hana::for_each(
            boost::hana::filter(held_,
                                []<typename T>(T&&) { return is_field<T>{}; }),
            std::forward<Func>(f));
    }

private:
    held_t held_;
};
} // namespace detail


}

