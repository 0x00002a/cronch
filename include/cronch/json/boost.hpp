
#pragma once

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

template<typename T, typename C>
concept boost_json_converter = requires(const boost::json::value& dt, T& v, boost::json::value& f, const T& v2)
{
    {C::to_json(dt, v)};
    {C::from_json(f, v2)};
};

} // namespace detail

class boost {
public:
    template<typename T>
    struct converter {
    };

    using document_type = ::boost::json::value;

    explicit boost(const std::string& doc) : doc_{::boost::json::parse(doc)} {}
    explicit boost(document_type doc) : doc_{std::move(doc)} {}

    template<typename V>
        requires(cronch::concepts::meta_complete<V> || cronch::concepts::container<V> ||
                 detail::native_json_type<V> ||
                 detail::boost_json_converter<V, converter<V>>)
    static void append(document_type& doc, const V& v)
    {
        if constexpr (cronch::concepts::meta_complete<V>) {
            auto& element = doc.emplace_object();
            meta::accessors<V>().map([&]<typename A>(const A& accessor) mutable {
                document_type eldoc;
                append(eldoc, accessor(v));
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
                append(eldoc, v);
                element.emplace_back(std::move(eldoc));
            });
        }
    }

    template<cronch::concepts::meta_complete V>
    void parse_into(V& v) const
    {
        auto& doc = doc_.as_object();
        meta::accessors<V>().map(
            [&]<typename A>(const A& accessor) mutable { accessor(v, gen_obj<A>(doc.at(accessor.name))); });
    }

    template<cronch::concepts::serializable V>
        requires(!cronch::concepts::meta_complete<V>)
    void parse_into(V& v) const
    {
        const auto name = meta::nameof<V>();
        v = doc_.as_object().at(name);
    }
    template<cronch::concepts::container V>
        requires(!cronch::concepts::serializable<V>)
    void parse_into(V& v) const
    {
        auto& doc = doc_.as_array();
        std::transform(std::begin(doc), std::end(doc), std::back_inserter(v),
                       [&](const auto& val) mutable { return gen_obj<V>(val); });
    }

    static auto to_string(const document_type& doc) -> std::string { return ::boost::json::serialize(doc); }

private:
    template<typename V>
    auto gen_obj(document_type doc) -> typename V::value_type
    {
        using T = typename V::value_type;
        T obj;
        boost{std::move(doc)}.parse_into(obj);
        return obj;
    }

    document_type doc_;
};

template<std::integral T>
struct boost::converter<T> {
    static void to_json(::boost::json::value& doc, const T& v) { doc.as_int64() = v; }
    static void from_json(const ::boost::json::value& doc, T& v) { v = static_cast<T>(doc.as_int64()); }
};

template<>
struct boost::converter<std::string> {
    static void to_json(::boost::json::value& doc, const std::string& v) { doc.as_string() = v.c_str(); }
    static void from_json(const ::boost::json::value& doc, std::string& v) { v = doc.as_string().c_str(); }
};
} // namespace cronch::json
