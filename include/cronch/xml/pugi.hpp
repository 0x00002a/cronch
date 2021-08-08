
#pragma once

#include <pugixml.hpp>

#include <boost/lexical_cast.hpp>
#include <sstream>

#include "cronch/concepts.hpp"
#include "cronch/meta/reflect.hpp"
#include "cronch/metadata.hpp"

namespace cronch::xml {

namespace detail {

template<typename T>
concept from_stringable = requires(const std::string& str)
{
    boost::lexical_cast<T>(str);
};

template<typename T>
concept pugi_xml_serializable = (cronch::concepts::iterable<T> || cronch::concepts::has_members<T> ||
                                 cronch::concepts::ostreamable<T>) &&cronch::concepts::serializable<T>;

template<typename T>
concept pugi_xml_deserializable = cronch::concepts::serializable<T> &&
    (from_stringable<T> || cronch::concepts::has_members<T> || cronch::concepts::container<T>);

template<bool attrMode>
class basic_xml {
public:
    using document_type = pugi::xml_document;

    explicit basic_xml(const std::string& contents) : doc_{} { doc_.load_string(contents.c_str()); }
    explicit basic_xml(pugi::xml_document doc) : doc_{std::move(doc)} {}

    template<pugi_xml_serializable V>
    static void serialize_to(pugi::xml_node& top, const V& outer_value)
    {
        if constexpr (cronch::concepts::serializable<V>) {
            if (top.name() != std::string_view{meta::nameof<V>()}) {
                auto el = top.append_child(meta::nameof<V>());
                serialize_to(el, outer_value);
                return;
            }
        }
        auto append = [&](const auto& v) mutable {
            if constexpr (cronch::concepts::has_members<V>) {
                meta::accessors<V>().map([&](auto&& f) mutable {
                    auto name = f.name;
                    auto&& read_val = f(v);

                    auto new_doc = top.append_child(name.data());
                    serialize_to(new_doc, read_val);
                });
            }
            else if constexpr (concepts::ostreamable<V>) {
                top.append_child(pugi::node_pcdata).set_value(boost::lexical_cast<std::string>(v).c_str());
            }
        };
        if constexpr (cronch::concepts::iterable<V>) {
            std::for_each(std::begin(outer_value), std::end(outer_value), append);
        }
        else {
            append(outer_value);
        }
    }

    template<pugi_xml_deserializable V>
    void deserialize_to(V& out)
    {
        // Traverse down to our node
        if constexpr (cronch::concepts::serializable<V>) {
            if (doc_.name() != std::string_view{meta::nameof<V>()}) {
                basic_xml{doc_.child(meta::nameof<V>())}.deserialize_to(out);
                return;
            }
        }

        if constexpr (cronch::concepts::has_members<V>) {
            meta::accessors<V>().map([&](auto&& f) mutable {
                auto fname = f.name;

                using vtype = typename std::decay_t<decltype(f)>::value_type;
                vtype value;
                if constexpr (concepts::serializable<vtype>) {
                    basic_xml{doc_.child(fname.data())}.deserialize_to(value);
                }
                else if constexpr (attrMode) {
                    value = boost::lexical_cast<vtype>(doc_.attribute(fname.data()).value());
                }
                else {
                    value = boost::lexical_cast<vtype>(doc_.child(fname.data()).text());
                }
                f(out, value);
            });
        }
        else if constexpr (from_stringable<V>) {
            out = boost::lexical_cast<V>(doc_.value());
        }
        else if constexpr (cronch::concepts::container<V>) {
            std::transform(std::begin(doc_), std::end(doc_), std::back_inserter(out), [](const auto& node) {
                using T = typename V::value_type;
                T new_val;
                basic_xml{node}.deserialize_to(new_val);
                return new_val;
            });
        }
    }

    static auto to_string(const document_type& doc) -> std::string
    {
        std::stringstream ss;
        doc.save(ss);
        return ss.str();
    }

private:
    static void append_value(pugi::xml_node& doc, std::string_view name, const std::string& value)
    {
        if constexpr (attrMode) {
            doc.append_attribute(name.data()).set_value(value.c_str());
        }
        else {
            doc.append_child(name.data()).append_child(pugi::node_pcdata).set_value(value.c_str());
        }
    }

    document_type doc_;
};

} // namespace detail

using pugi = detail::basic_xml<false>;

} // namespace cronch::xml
