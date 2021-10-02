
#pragma once

#ifndef CRONCH_FEATURE_BOOST_JSON
#error "cronch must be build with CRONCH_FEATURE_BOOST_JSON support to enable this header"
#endif

#include "cronch/meta/reflect.hpp"
#include <boost/json.hpp>

#include <cronch/concepts.hpp>

namespace cronch::json {
namespace detail {
template<typename T>
concept native_json_type = requires(const T& t)
{
    boost::json::value(t);
};

template<typename T>
concept from_json_type = requires(T& t) {
    t = boost::json::value{};
};

template<typename T, typename C>
concept boost_json_converter = requires(const boost::json::value& dt, T& v, boost::json::value& f, const T& v2)
{
    {C::to_json(f, v2)};
    {C::from_json(dt, v)};
};

template<typename V, template<typename> typename C>
concept boost_json_supported_type =  cronch::concepts::meta_complete<V> || cronch::concepts::container<V> || native_json_type<V> ||
                boost_json_converter<V, C<V>>;

} // namespace detail

class boost {
public:
    template<typename T>
    struct converter {
    };

    using document_type = ::boost::json::value;

    explicit boost(const std::string& doc) : doc_(::boost::json::parse(doc)) {}
    explicit boost(document_type doc) : doc_(std::move(doc)) {}

    template<detail::boost_json_supported_type<converter> V>
    static void serialize_to(document_type& doc, const V& v)
    {
        if constexpr (cronch::concepts::meta_complete<V>) {
            auto& element = doc.emplace_object();
            meta::accessors<V>().map([&]<typename A>(const A& accessor) mutable {
                document_type eldoc;
                serialize_to(eldoc, accessor(v));
                element.emplace(accessor.name.data(), std::move(eldoc));
            });
        }
        else if constexpr (detail::native_json_type<V>) {
            doc = document_type(v);
        }
        else if constexpr (detail::boost_json_converter<V, converter<V>>) {
            converter<V>::to_json(doc, v);
        }
        else if constexpr (cronch::concepts::container<V>) {
            auto& element = doc.emplace_array();
            std::for_each(std::begin(v), std::end(v), [&](auto& v) mutable {
                document_type eldoc;
                serialize_to(eldoc, v);
                element.emplace_back(std::move(eldoc));
            });
        }
    }

    template<detail::boost_json_supported_type<converter> V>
    void deserialize_to(V& v) const
    {
        if constexpr (cronch::concepts::meta_complete<V>) {
            auto& doc = doc_.as_object();
            meta::accessors<V>().map(
                [&]<typename A>(const A& accessor) mutable { accessor(v, gen_obj<A>(doc.at(accessor.name.data()))); });
        }
        else if constexpr (detail::from_json_type<V>) {
            v = doc_;
        }
        else if constexpr (detail::boost_json_converter<V, converter<V>>) {
            converter<V>::from_json(doc_, v);
        }
        else if constexpr (cronch::concepts::container<V>) {
            auto& doc = doc_.as_array();
            std::transform(std::begin(doc), std::end(doc), std::back_inserter(v),
                           [&](const auto& val) mutable { return gen_obj<V>(val); });
        }
    }

    static auto to_string(const document_type& doc) -> std::string { return ::boost::json::serialize(doc); }

private:
    template<typename V>
    auto gen_obj(const document_type& doc) const -> typename V::value_type
    {
        using T = typename V::value_type;
        T obj;
        boost{doc}.deserialize_to(obj);
        return obj;
    }

    document_type doc_;
};
} // namespace cronch::json

template<std::integral T>
struct cronch::json::boost::converter<T> {
    static void to_json(::boost::json::value& doc, const T& v) { doc.as_int64() = v; }
    static void from_json(const ::boost::json::value& doc, T& v) { v = static_cast<T>(doc.as_int64()); }
};

#if __GNUC__ && !__llvm__ // workaround for: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92944
#define CRONCH_DBL_CONV(type)                                                                                          \
    template<>                                                                                                         \
    struct cronch::json::boost::converter<type> {                                                                      \
        using V = type;                                                                                                \
        static void to_json(::boost::json::value& doc, const V& v) { doc.as_double() = v; }                            \
        static void from_json(const ::boost::json::value& doc, V& v) { v = static_cast<V>(doc.as_double()); }          \
    };

CRONCH_DBL_CONV(float);
CRONCH_DBL_CONV(double);
CRONCH_DBL_CONV(long double);

#undef CRONCH_DBL_CONV

#else
template<std::floating_point V>
struct cronch::json::boost::converter<V> {
    static void to_json(::boost::json::value& doc, const V& v) { doc.as_double() = v; }
    static void from_json(const ::boost::json::value& doc, V& v) { v = static_cast<V>(doc.as_double()); }
};
#endif
template<>
struct cronch::json::boost::converter<std::string> {
    static void to_json(::boost::json::value& doc, const std::string& v) { doc.as_string() = v.c_str(); }
    static void from_json(const ::boost::json::value& doc, std::string& v) { v = doc.as_string().c_str(); }
};

static_assert(cronch::json::detail::boost_json_converter<std::string, cronch::json::boost::converter<std::string>>);
static_assert(cronch::json::detail::boost_json_converter<int, cronch::json::boost::converter<int>>);
static_assert(cronch::json::detail::boost_json_converter<float, cronch::json::boost::converter<float>>);
static_assert(cronch::json::detail::boost_json_converter<double, cronch::json::boost::converter<double>>);
