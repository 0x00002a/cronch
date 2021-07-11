
#pragma once

#include "cronch/metadata.hpp"

#include <concepts>
#include <string_view>

namespace cronch::concepts::detail {
struct empty_serializable {
};

} // namespace cronch::concepts::detail
template<>
struct cronch::metadata<cronch::concepts::detail::empty_serializable> {
    static constexpr auto name = "";
};
namespace cronch::concepts {

template<typename T, template<typename...> typename Against>
concept is = requires(const T& v)
{
    []<typename... Args>(const Against<Args...>&){}(v);
};

template<typename S>
concept has_fields = requires() {
    { metadata<S>::fields };
};

template<typename C>
concept iterable = requires(C c) {
    { c.begin() };
    { c.end() };
};

template<typename S>
concept serializable = requires(const S& inst)
{
    {metadata<S>::name};
};

template<typename B>
concept backend = requires(typename B::document_type& doc)
{
    B::append(doc, detail::empty_serializable{});
};

} // namespace cronch::concepts
