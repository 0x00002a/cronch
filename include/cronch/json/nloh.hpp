
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
    requires(concepts::json_serializable<V> && !cronch::concepts::known_to_cronch<V>) static void append(
        document_type& doc, const V& val)
    {
        doc = val;
    }

    template<cronch::concepts::known_to_cronch V>
    static void append(document_type& doc, const V& v)
    {
        meta::accessors<V>().map([&](auto&& f) mutable {
            auto name = f.name;
            
            using vtype = typename std::decay_t<decltype(f)>::value_type;
            auto&& value = f(v);

            if constexpr (cronch::concepts::serializable<vtype>) {
                auto& subdoc = doc[std::string(name)];
                
                append(subdoc, value);
            }
            else {
                doc[std::string(name)] = value;
            }
        });
    }

    template<concepts::json_deserializable V>
    requires(!cronch::concepts::known_to_cronch<V>) void parse_into(
        V& out) const
    {
        doc_.get_to(out);
    }
    template<cronch::concepts::iterable V>
    requires(!cronch::concepts::has_members<V> &&
             !concepts::json_deserializable<V>) void parse_into(V& out) const
    {
        auto i = 0ul;
        for (auto& v : out) {
            nloh sub{doc_.at(i)};
            sub.parse_into(v);
            ++i;
        }
    }

    template<cronch::concepts::known_to_cronch V>
    void parse_into(V& out) const
    {
        meta::accessors<V>().map([&](auto&& f) mutable {
            auto name = f.name;

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            vtype value;
            if constexpr (cronch::concepts::serializable<vtype>) {
                nlohmann::json sub_doc = doc_.at(std::string{name});
                nloh sub{std::move(sub_doc)};
                sub.parse_into(value);
            }
            else {
                doc_.at(std::string{name}).get_to(value);
            }
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
