
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

    template<concepts::iterable V>
    requires (!concepts::serializable<V> && !concepts::ostreamable<V>)
    static void append(pugi::xml_node& doc, std::string_view name, const V& v)
    {
        auto top = doc.append_child(name.data());
        for (const auto& child : v) {
            append(top, child);
        }
    }

    template<concepts::serializable V>
    static void append(pugi::xml_node& doc, std::string_view name, const V& v)
    {
        auto top = doc.append_child(name.data());
        for (const auto& child : v) {
            append(top, child);
        }
    }
    template<concepts::iterable V>
    requires(!concepts::has_fields<V> && concepts::serializable<V>) 
    static void append(pugi::xml_node& doc, const V& v)
    {
        append(doc, metadata<V>::name, v);
    }

    template<concepts::ostreamable V>
    requires(!concepts::has_fields<V>) 
    static void append(pugi::xml_node& doc, std::string_view name, const V& v)
    {
        append_value(doc, name,
                     boost::lexical_cast<std::string>(v));
    }

    template<concepts::ostreamable V>
    requires(!concepts::has_fields<V> && concepts::serializable<V>) 
    static void append(pugi::xml_node& doc, const V& v)
    {
        append(doc, v, metadata<V>::name);
    }
    template<concepts::serializable V>
    requires(concepts::has_fields<V>) 
    static void append(pugi::xml_node& top, const V& v)
    {
        auto doc = top.append_child(metadata<V>::name);
        metadata<V>::fields.map([&](auto&& f) mutable {
            auto mem = f.mem_ref;
            auto name = f.name;
            const auto& value = v.*mem;

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            if constexpr (concepts::serializable<vtype>) {
                auto subdoc = doc.child(name.data());
                append(subdoc, value);
            }
            else {
                append(doc, name, value);
            }
        });
    }

    template<concepts::serializable V>
    void parse_into(V& out) const
    {
        metadata<V>::fields.map([&](auto&& f) mutable {
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
    static void append_value(pugi::xml_node& doc, std::string_view name,
                             const std::string& value)
    {
        if constexpr (attrMode) {
            doc.append_attribute(name.data()).set_value(value.c_str());
        }
        else {
            doc.append_child(name.data())
                .append_child(pugi::node_pcdata)
                .set_value(value.c_str());
        }
    }

    document_type doc_;
};

} // namespace detail
using pugi_attr = detail::basic_xml<true>;
using pugi = detail::basic_xml<false>;

} // namespace cronch::xml
