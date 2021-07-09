
#pragma once

#include "cronch/metadata.hpp"

namespace cronch::concepts {

template<typename T, typename Against>
    concept is = requires(const T& v) {
        []<typename... Args>(const Against<Args...>&) {}(v);
    };

template<typename S>
concept serializable = requires(const S& inst)
{
    using mdata_t = metadata<S>;
    constexpr auto v2 = mdata_t::about;
};

template<typename B>
concept backend = requires(typename B::document_type& doc)
{
    {
        doc.append(doc, {})
    }
};

} // namespace cronch::concepts
