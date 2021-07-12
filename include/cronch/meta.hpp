
#pragma once

#include <string_view>
#include <tuple>

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/container.hpp>
#include <boost/hana.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/vector.hpp>
#include <type_traits>

#include "cronch/concepts.hpp"

namespace cronch::meta {
struct name {
    std::string_view value;

    constexpr explicit name(std::string_view v) : value{v} {}
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
class fields_store {
    using held_t = boost::hana::tuple<Args...>;

public:
    constexpr explicit fields_store(Args... values) : held_{std::move(values)...}
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

template<typename... Args>
constexpr auto fields(Args&&... args)
{
    return detail::fields_store<std::decay_t<Args>...>{
        std::forward<Args>(args)...};
}
} // namespace cronch::meta

#define CRONCH_META_TYPE(type, nfields)                                        \
    namespace cronch {                                                         \
    template<>                                                                 \
    struct metadata<type> {                                                    \
        static constexpr const char* name = #type;                             \
        static constexpr auto fields = cronch::meta::fields nfields;           \
    };                                                                         \
    }
