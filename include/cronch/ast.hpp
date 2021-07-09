
#pragma once

#include <string_view>

namespace cronch {
namespace detail {

template<typename T, typename Vt>
struct stored_field {
    using field_t = Vt T::*;
    using owner_t = T;

    std::string_view name;
    field_t field;
};

} // namespace detail

class ast {
public:
};

} // namespace cronch
