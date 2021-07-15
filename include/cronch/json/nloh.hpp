
#pragma once

#include <nlohmann/json.hpp>

#include "cronch/concepts.hpp"
#include "cronch/json/concepts.hpp"
#include "cronch/metadata.hpp"
#include "cronch/meta/reflect.hpp"

#include <string>

namespace cronch::json {

class nloh {
public:
    using document_type = nlohmann::json;

    explicit nloh(const std::string& content)
        : doc_(nlohmann::json::parse(content))
    {
    }
    explicit nloh(document_type doc) : doc_(std::move(doc)) {}

    template<typename V>
    requires(concepts::json_serializable<V> && !cronch::concepts::meta_complete<V>)
    static void append(
        document_type& doc, const V& val)
    {
        doc = val;
    }
    template<cronch::concepts::iterable V>
    requires (!concepts::json_serializable<V> && !cronch::concepts::meta_complete<V>)
    static void append(document_type& doc, const V& val) {
        std::size_t i = 0;
        for(const auto& v : val) {
            nlohmann::json j;
            append(j, v);
            doc.emplace_back(j);
            ++i;
        }
    }

    template<cronch::concepts::meta_complete V>
    static void append(document_type& doc, const V& v)
    {
        meta::accessors<V>().map([&](auto&& f) mutable {
            auto name = f.name;
            auto&& value = f(v);

            auto& subdoc = doc[std::string(name)];
            append(subdoc, value);
        });
    }

    template<concepts::json_deserializable V>
    requires(!cronch::concepts::meta_complete<V>) void parse_into(
        V& out) const
    {
        doc_.get_to(out);
    }
    template<cronch::concepts::container V>
    requires(!cronch::concepts::has_members<V> &&
             !concepts::json_deserializable<V>) void parse_into(V& out) const
    {
        using vtype = typename std::decay_t<V>::value_type;
        std::transform(doc_.begin(), doc_.end(), std::back_inserter(out), [&](const auto& subdoc){
          nloh sub{subdoc};
          vtype v;
          sub.parse_into(v);
          return v;
        });
    }

    template<cronch::concepts::meta_complete V>
    void parse_into(V& out) const
    {
        meta::accessors<V>().map([&](auto&& f) mutable {
            auto name = f.name;

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            vtype value;
            nlohmann::json sub_doc = doc_.at(std::string{name});
            nloh sub{std::move(sub_doc)};
            sub.parse_into(value);

            f(out, value);
        });
    }

    static auto to_string(const document_type& doc) -> std::string
    {
        return doc.dump();
    }

private:
    document_type doc_;
};

static_assert(cronch::concepts::backend<nloh>, "nloh is invalid");

} // namespace cronch::json
