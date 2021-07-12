#include <cronch/meta.hpp>
#include <cronch/metadata.hpp>

struct my_type {
    int my_field;
};

namespace cronch {
template<>
struct metadata<my_type> {
    static constexpr auto name = "my_type";
    static constexpr auto fields = cronch::meta::fields(
        cronch::meta::field("my_field", &my_type::my_field));
};

} // namespace cronch
