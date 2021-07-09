
#pragma once

#include "cronch/metadata.hpp"

#include <concepts>

namespace cronch::concepts::detail {
struct empty_serializable {
};

} // namespace cronch::concepts::detail
template<>
struct cronch::metadata<cronch::concepts::detail::empty_serializable> {
    static constexpr auto about = 0;
};
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
    B::append(doc, detail::empty_serializable{});
};

} // namespace cronch::concepts
