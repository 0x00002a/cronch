
#pragma once

#include <nlohmann/json.hpp>

#include "cronch/concepts.hpp"
#include "cronch/json/concepts.hpp"
#include "cronch/meta/reflect.hpp"
#include "cronch/metadata.hpp"

#include <string>

namespace cronch::json {
namespace detail {
template<typename T>
concept nloh_supported_type = cronch::concepts::has_members<T> || cronch::concepts::serializable<T>;

template<typename T>
concept nloh_supported_type_serial =
    nloh_supported_type<T> || concepts::json_serializable<T> || cronch::concepts::iterable<T>;
template<typename T>
concept nloh_supported_type_deserial =
    nloh_supported_type<T> || concepts::json_deserializable<T> || cronch::concepts::container<T>;
} // namespace detail

class nloh {
public:
    using document_type = nlohmann::json;

    explicit nloh(const std::string& content) : doc_(nlohmann::json::parse(content)) {}
    explicit nloh(document_type doc) : doc_(std::move(doc)) {}

    template<detail::nloh_supported_type_serial V>
    static void serialize_to(document_type& doc, const V& val)
    {
        if constexpr (cronch::concepts::has_members<V>) {

            meta::accessors<V>().map([&](auto&& f) mutable {
                auto name = f.name;
                auto&& value = f(val);

                auto& subdoc = doc[std::string(name)];
                serialize_to(subdoc, value);
            });
        }
        else if constexpr (concepts::json_serializable<V>) {
            doc = val;
        }
        else if constexpr (cronch::concepts::iterable<V>) {
            std::size_t i = 0;
            if (std::begin(val) == std::end(val)) {
                // Empty
                doc = nlohmann::json::parse("[]");
            }
            else {
                for (const auto& v : val) {
                    nlohmann::json j;
                    serialize_to(j, v);
                    doc.emplace_back(j);
                    ++i;
                }
            }
        }
    }

    template<detail::nloh_supported_type_deserial V>
    void deserialize_to(V& val) const
    {
        if constexpr (cronch::concepts::meta_complete<V>) {

            meta::accessors<V>().map([&](auto&& f) mutable {
                auto name = f.name;

                using vtype = typename std::decay_t<decltype(f)>::value_type;
                vtype value;
                nlohmann::json sub_doc = doc_.at(std::string{name});
                nloh sub{std::move(sub_doc)};
                sub.deserialize_to(value);

                f(val, value);
            });
        }
        else if constexpr (concepts::json_deserializable<V>) {
            doc_.get_to(val);
        }
        else if constexpr (cronch::concepts::container<V>) {

            using vtype = typename std::decay_t<V>::value_type;
            std::transform(doc_.begin(), doc_.end(), std::back_inserter(val), [&](const auto& subdoc) {
                nloh sub{subdoc};
                vtype v;
                sub.deserialize_to(v);
                return v;
            });
        }
    }

    static auto to_string(const document_type& doc) -> std::string { return doc.dump(); }

private:
    document_type doc_;
};

static_assert(cronch::concepts::backend<nloh>, "nloh is invalid");

} // namespace cronch::json
