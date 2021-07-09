
#pragma once

#include <string_view>
#include <tuple>

#include <boost/mpl/vector.hpp>

#include "cronch/concepts.hpp"

namespace cronch::meta {
struct name {
    std::string_view value;
};

template<typename T, typename Vt>
struct field {
    using field_t = Vt T::*;
    using owner_t = T;

    std::string_view name;
    field_t mem_ref;

    constexpr field(std::string_view name_, field_t mem)
        : name{name_}, mem_ref{mem}
    {
    }
};

namespace detail {

template<typename T>
constexpr bool is_name = std::same_as<T, name>;

template<typename T>
    struct is_field {
        enum { 
            value = concepts::is<T, field>
        };
    };

template<typename... Args>
class about_store {
    using held_t = std::tuple<Args...>;
    using held_vec = boost::mpl::vector<Args...>;

public:
    constexpr explicit about_store(held_t held) : held_{std::move(held)} {}

    template<typename T, typename Func>
    constexpr void map_fields() {

    }

    constexpr auto name() const -> std::string_view
    {
        using idx = boost::mpl::find<held_vec, name>::type;
        return std::get<idx>(held_).value;
    }

private:
    held_t held_;
};
} // namespace detail

template<typename... Args>
constexpr auto about(Args&&... args)
{
    return detail::about_store<std::decay_t<Args>...>{
        std::tuple<std::decay_t<Args>...>{std::forward<Args>(args)...}};
}

} // namespace cronch::meta
