
#pragma once

#include <nlohmann/json.hpp>

#include "cronch/concepts.hpp"
#include "cronch/metadata.hpp"
#include "cronch/json/concepts.hpp"

#include <string>

namespace cronch::json {

class nloh {
public:
    using document_type = nlohmann::json;

    explicit nloh(const std::string& content)
        : doc_(nlohmann::json::parse(content))
    {
    }

    template<cronch::concepts::serializable V>
    requires (concepts::json_serializable<V>) 
    static void append(document_type& doc, const V& val) {
        doc = val;
    }


    template<cronch::concepts::serializable V>
    requires (cronch::concepts::has_fields<V>)
    static void append(document_type& doc, const V& v)
    {
        metadata<V>::fields.map([&](auto&& f) mutable {
            auto mem = f.mem_ref;
            auto name = f.name;
            const auto& value = v.*mem;

            using vtype = typename std::decay_t<decltype(f)>::value_type;
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
    requires (!cronch::concepts::has_fields<V> && !cronch::concepts::serializable<V>)
    void parse_into(V& out) const {
        doc_.get_to(out);
    }
    template<cronch::concepts::iterable V>
    requires (!cronch::concepts::known_to_cronch<V>)
    void parse_into(V& out) {
        for (auto& v : out) {
            parse_into(v);
        }
    }

    template<cronch::concepts::serializable V>
    requires (cronch::concepts::has_fields<V>)
    void parse_into(V& out) const
    {
        if constexpr (cronch::concepts::has_fields<V>) {
            metadata<V>::fields.map([&](auto&& f) mutable {
                auto mem = f.mem_ref;
                auto name = f.name;
                auto& value = out.*mem;

                using vtype = typename std::decay_t<decltype(f)>::value_type;
                if constexpr (cronch::concepts::serializable<vtype>) {
                    nloh sub{doc_.at(std::string{name})};
                    sub.parse_into(value);
                }
                else {
                    doc_.at(std::string{name}).get_to(value);
                }
            });
        }
        else if constexpr (concepts::json_deserializable<V>) {
            doc_.get_to(out);
        }
        else {
            static_assert(detail::always_false<V>, "Not convertible");
        }
    }

    static auto to_string(const document_type& doc) -> std::string
    {
        return doc.dump();
    }

private:
    document_type doc_;
};

} // namespace cronch
