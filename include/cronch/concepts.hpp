
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
concept has_members = requires() {
    { metadata<S>::members };
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

template<typename S>
concept known_to_cronch = has_members<S> && serializable<S>;


template<typename C>
concept ostreamable = requires(C c, std::ostream& s) {
    s << c;
};

template<typename B>
concept backend = requires(typename B::document_type& doc, const B& b, detail::empty_serializable& s, const detail::empty_serializable& cs)
{
    { B::append(doc, cs) };
    { b.parse_into(s) };
};

} // namespace cronch::concepts
