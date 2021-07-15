
#pragma once

#include <pugixml.hpp>

#include <boost/lexical_cast.hpp>
#include <sstream>

#include "cronch/concepts.hpp"
#include "cronch/metadata.hpp"
#include "cronch/meta/reflect.hpp"

namespace cronch::xml {

namespace detail {

template<bool attrMode>
class basic_xml {
public:
    using document_type = pugi::xml_document;

    explicit basic_xml(const std::string& contents) : doc_{} {
        doc_.load_string(contents.c_str());
    }
    explicit basic_xml(pugi::xml_document doc) : doc_{std::move(doc)} {}


    template<cronch::concepts::iterable V>
    requires (!cronch::concepts::has_members<V> && !concepts::ostreamable<V>)
    static void append(pugi::xml_node& doc, std::string_view name, const V& v)
    {
        auto top = doc.append_child(name.data());
        for (const auto& child : v) {
            append(top, child);
        }
    }
    

    template<concepts::ostreamable V>
    requires(!concepts::has_members<V>) 
    static void append(pugi::xml_node& doc, std::string_view name, const V& v)
    {
        append_value(doc, name,
                     boost::lexical_cast<std::string>(v));
    }

    template<typename V>
    requires(!concepts::has_members<V> && concepts::serializable<V>) 
    static void append(pugi::xml_node& doc, const V& v)
    {
        append(doc, meta::nameof<V>(), v);
    }
    template<concepts::serializable V>
    requires(concepts::has_members<V>) 
    static void append(pugi::xml_node& top, const V& v)
    {
        auto doc = top.append_child(meta::nameof<V>());
        meta::accessors<V>().map([&](auto&& f) mutable {
            auto name = f.name;
            auto&& value = f(v);

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            if constexpr (concepts::serializable<vtype>) {
                append(doc, name, value);
            }
            else {
                append(doc, name, value);
            }
        });
    }

    template<cronch::concepts::meta_complete V>
    void parse_into(V& out) const {
        parse_into(out, meta::nameof<V>());
    }

    template<cronch::concepts::has_members V>
    void parse_into(V& out, std::string_view name) const
    {
        auto doc = doc_.child(name.data());
        meta::accessors<V>().map([&](auto&& f) mutable {
            auto fname = f.name;

            using vtype = typename std::decay_t<decltype(f)>::value_type;
            vtype value;
            if constexpr (concepts::serializable<vtype>) {
                basic_xml{doc}.parse_into(value, fname);
            }
            else if constexpr (attrMode) {
                value = boost::lexical_cast<vtype>(
                    doc_.attribute(fname.data()).value());
            }
            else {
                value =
                    boost::lexical_cast<vtype>(doc_.child(fname.data()).text());
            }
            f(out, value);
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

using pugi = detail::basic_xml<false>;

} // namespace cronch::xml
