
#include <iostream>

#include <cronch/json/json.hpp>
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
    constexpr static auto fields = cr::meta::fields(
        cr::meta::field("name", &my_type::name),
        cr::meta::field("some_info", &my_type::some_info)
    );
    // clang-format on
};

int main()
{
    const auto my_obj = my_type{"Hello there", 4};
    std::cout << "my_type: " << cr::serialize<cr::json::nloh>(my_obj) << '\n';

}
