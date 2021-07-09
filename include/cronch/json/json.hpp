
#pragma once

#include <nlohmann/json.hpp>

#include "cronch/concepts.hpp"
#include "cronch/metadata.hpp"

#include <string>

namespace cronch {

class json {
public:
    using document_type = nlohmann::json;

    explicit json(const std::string& content)
        : doc_{nlohmann::json::parse(content)}
    {
    }

    template<concepts::serializable V>
    static void append(document_type& doc, const V& v)
    {
        metadata<V>::about.map_fields([&](auto&& f) mutable {
            auto mem = f.mem_ref;
            auto name = f.name;
            const auto& value = v.*mem;

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            if constexpr (concepts::serializable<vtype>) {
                auto& subdoc = doc[std::string(name)];
                append(subdoc, value);
            }
            else {
                doc[std::string(name)] = value;
            }
        });
    }
    template<concepts::serializable V>
    void parse_into(V& out) const 
    {
        metadata<V>::about.map_fields([&](auto&& f) mutable {
            auto mem = f.mem_ref;
            auto name = f.name;
            auto& value = out.*mem;

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            if constexpr (concepts::serializable<vtype>) {
                json sub{doc_.at(std::string{name})};
                sub.parse_into(value);
            }
            else {
                doc_.at(std::string{name}).get_to(value);
            }
        });
    }

    static auto to_string(const document_type& doc) -> std::string
    {
        return doc.dump();
    }

private:
    document_type doc_;
};

} // namespace cronch
