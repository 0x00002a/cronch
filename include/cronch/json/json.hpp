
#pragma once

#include <nlohmann/json.hpp>

#include "cronch/concepts.hpp"
#include "cronch/metadata.hpp"

#include <string>

namespace cronch {

struct json {
    using document_type = nlohmann::json;

    template<concepts::serializable V>
    static void append(document_type& doc, const V& v)
    {
        metadata<V>::about.map_fields([&](auto&& f) mutable {
            auto mem = f.mem_ref;
            auto name = f.name;
            doc[std::string(name)] = v.*mem;
        });
    }

    static auto to_string(const document_type& doc) -> std::string
    {
        return doc.dump();
    }
};

} // namespace cronch
