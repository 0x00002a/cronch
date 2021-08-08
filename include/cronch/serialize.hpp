
#pragma once

#include <string>

#include <cronch/concepts.hpp>

namespace cronch {

template<concepts::serialization_backend Backend, typename From>
auto serialize(const From& from, typename Backend::document_type& with)
    -> std::string
{
    Backend::append(with, from);

    return Backend::to_string(with);
}
template<concepts::serialization_backend Backend, typename From>
requires requires(const typename Backend::document_type& doc) {
    { Backend::to_string(doc) } -> std::convertible_to<std::string>;
}
auto serialize(const From& from)
    -> std::string
{
    typename Backend::document_type with;
    Backend::append(with, from);

    return Backend::to_string(with);
}
} // namespace cronch
