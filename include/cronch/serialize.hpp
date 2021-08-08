
#pragma once

#include <string>

#include <cronch/concepts.hpp>

namespace cronch {

template<concepts::serialization_backend Backend, typename From>
void serialize(const From& from, typename Backend::document_type& to)
{
    Backend::serialize_to(to, from);
}
template<concepts::serialization_backend Backend, typename From>
requires requires(const typename Backend::document_type& doc) {
    { Backend::to_string(doc) } -> std::convertible_to<std::string>;
}
auto serialize(const From& from)
    -> std::string
{
    typename Backend::document_type with;
    Backend::serialize_to(with, from);

    return Backend::to_string(with);
}
} // namespace cronch
