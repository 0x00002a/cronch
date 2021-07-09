
#pragma once

#include <string_view>
#include <tuple>

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/container.hpp>
#include <boost/hana.hpp>
#include <boost/mpl/find.hpp>
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
    using value_type = Vt;

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
    enum { value = concepts::is<T, field> };
};

template<typename... Args>
class about_store {
    using held_t = boost::hana::tuple<Args...>;
    using held_vec = boost::mpl::vector<Args...>;

public:
    constexpr explicit about_store(Args... values) : held_{std::move(values)...}
    {
    }

    template<typename Func>
    constexpr void map_fields(Func&& f) const
    {
        boost::hana::for_each(
            boost::hana::filter(held_,
                                []<typename T>(T&&) { return is_field<T>{}; }),
            std::forward<Func>(f));
    }

    constexpr auto name() const -> std::string_view
    {
        using idx = boost::mpl::find<held_vec, cronch::meta::name>::type;
        return boost::fusion::at<idx>(held_).value;
    }

private:
    held_t held_;
};
} // namespace detail

template<typename... Args>
constexpr auto info(Args&&... args)
{
    return detail::about_store<std::decay_t<Args>...>{
        std::forward<Args>(args)...};
}
} // namespace cronch::meta
