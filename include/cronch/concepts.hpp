
#pragma once

#include "cronch/metadata.hpp"

#include <concepts>

namespace cronch::concepts {

template<typename T, template<typename...> typename Against>
concept is = requires(const T& v)
{
    []<typename... Args>(const Against<Args...>&){}(v);
};

template<typename S>
concept serializable = requires(const S& inst)
{
    {metadata<S>::about};
};

template<typename B>
concept backend = requires(typename B::document_type& doc)
{
    {doc.append(doc, {})};
};

} // namespace cronch::concepts
