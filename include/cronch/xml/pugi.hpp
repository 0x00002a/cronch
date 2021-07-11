
#pragma once

#include <pugixml.hpp>

#include <boost/lexical_cast.hpp>
#include <sstream>

#include "cronch/concepts.hpp"
#include "cronch/metadata.hpp"

namespace cronch::xml {

namespace detail {

template<bool attrMode>
class basic_xml {
public:
    using document_type = pugi::xml_document;

    template<concepts::serializable V>
    static void append(document_type& top, const V& v)
    {
        auto doc = top.append_child(metadata<V>::about.name().data());
        metadata<V>::about.map_fields([&](auto&& f) mutable {
            auto mem = f.mem_ref;
            auto name = f.name;
            auto value = boost::lexical_cast<std::string>(v.*mem);

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            if constexpr (concepts::serializable<vtype>) {
                auto subdoc = doc.child(name.data());
                append(subdoc, value);
            }
            else if constexpr (attrMode) {
                doc.append_attribute(name.data()).set_value(value.c_str());
            }
            else {
                doc.append_child(name.data())
                    .append_child(pugi::node_pcdata)
                    .set_value(value.c_str());
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
                auto sub = doc_.child(name.data());
                basic_xml{sub}.parse_into(value);
            }
            else if constexpr (attrMode) {
                value = boost::lexical_cast<vtype>(
                    doc_.attribute(name.data()).value());
            }
            else {
                value =
                    boost::lexical_cast<vtype>(doc_.child(name.data()).text());
            }
        });
    }

    static auto to_string(const document_type& doc) -> std::string
    {
        std::stringstream ss;
        doc.save(ss);
        return ss.str();
    }

private:
    document_type doc_;
};

} // namespace detail
using pugi_attr = detail::basic_xml<true>;
using pugi = detail::basic_xml<false>;

} // namespace cronch
