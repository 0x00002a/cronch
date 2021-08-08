
#pragma once

#include "cronch/metadata.hpp"

#include <iterator>
#include <concepts>
#include <string_view>

namespace cronch::concepts::detail {
struct empty_serializable {
};

struct empty_view {
    void map(auto&&) const {}
};
} // namespace cronch::concepts::detail
template<>
struct cronch::metadata<cronch::concepts::detail::empty_serializable> {
    static constexpr auto name = "";
    static constexpr auto members = cronch::concepts::detail::empty_view{};
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
    { std::begin(c) };
    { std::end(c) };
};

template<typename C>
concept container = iterable<C> && requires(const C& c) {
    typename C::value_type;
    { std::back_inserter(c) };
};

template<typename S>
concept serializable = requires(const S& inst)
{
    {metadata<S>::name};
};

template<typename S>
concept meta_complete = has_members<S> && serializable<S>;

static_assert(meta_complete<detail::empty_serializable>);

template<typename C>
concept ostreamable = requires(C c, std::ostream& s) {
    s << c;
};

template<typename B>
concept serialization_backend = requires(typename B::document_type& doc, const detail::empty_serializable& cs) {
    { B::serialize_to(doc, cs) };
};
template<typename B>
concept deserizalation_backend = requires(const B& b, detail::empty_serializable& s) {
    { b.deserialize_to(s) };
};

template<typename B>
concept backend = serialization_backend<B> && deserizalation_backend<B>;

} // namespace cronch::concepts
