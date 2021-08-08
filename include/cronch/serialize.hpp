
#pragma once

#include <string>

#include <cronch/concepts.hpp>

namespace cronch {

template<concepts::backend Backend, typename From>
auto serialize(const From& from, typename Backend::document_type& with)
    -> std::string
{
    Backend::append(with, from);

    return Backend::to_string(with);
}
template<concepts::backend Backend, typename From>
auto serialize(const From& from)
    -> std::string
{
    typename Backend::document_type with;
    Backend::append(with, from);

    return Backend::to_string(with);
}
} // namespace cronch
