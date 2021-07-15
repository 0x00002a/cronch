
#include <iostream>

#include <cronch/json/nloh.hpp>
#include <cronch/meta.hpp>
#include <cronch/serialize.hpp>

#include <string>

namespace cr = cronch;

struct my_type {
    std::string name;
    int some_info;
};

template<>
struct cr::metadata<my_type> {
    // clang-format off
    constexpr static auto name = "my_type";
    constexpr static auto members = cr::meta::mems(
        cr::meta::field("name", &my_type::name),
        cr::meta::field("some_info", &my_type::some_info)
    );
    // clang-format on
};
static_assert(!cr::json::concepts::json_serializable<std::vector<my_type>>);

int main()
{
    const auto my_obj = my_type{"Hello there", 4};
    std::cout << "my_type: " << cr::serialize<cr::json::nloh>(my_obj) << '\n';
    std::vector<my_type> types{{"Hello", 2}, {"Sup", 3}};
    std::cout << "my_types: " << cr::serialize<cr::json::nloh>(types) << '\n';

}
