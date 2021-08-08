
#pragma once

#include "cronch/meta/reflect.hpp"
#include <boost/json.hpp>

#include <cronch/concepts.hpp>

namespace cronch::json {
namespace detail {
template<typename T>
concept native_json_type = requires(const T& t)
{
    boost::json::value{t};
};

} // namespace detail

class boost {
public:
    using document_type = ::boost::json::value;

    explicit boost(const std::string& doc) : doc_{::boost::json::parse(doc)} {}
    explicit boost(document_type doc) : doc_{std::move(doc)} {}

    template<cronch::concepts::container V>
        requires(!cronch::concepts::serializable<V> && !detail::native_json_type<V>)
    static void append(document_type& doc, const V& v)
    {
        auto element = doc.emplace_array();
        std::for_each(std::begin(v), std::end(v), [&](auto& v) mutable {
            document_type eldoc;
            append(eldoc, v);
            element.emplace_back(std::move(eldoc));
        });
    }

    template<cronch::concepts::serializable V>
        requires(!cronch::concepts::meta_complete<V>)
    static void append(document_type& doc, const V& v) { doc = v; }

    template<cronch::concepts::meta_complete V>
    static void append(document_type& doc, const V& v)
    {
        auto element = doc.emplace_object();
        meta::accessors<V>().map([&]<typename A>(const A& accessor) mutable {
            document_type eldoc;
            append(eldoc, accessor(v));
            element.emplace(accessor.name.data(), std::move(eldoc));
        });
    }
    template<detail::native_json_type V>
    requires (!cronch::concepts::serializable<V>)
    static void append(document_type& doc, V&& v) {
        doc = document_type{std::forward<V>(v)};
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

    static auto to_string(const document_type& doc) -> std::string {
        return ::boost::json::serialize(doc);
    }

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

} // namespace cronch::json
