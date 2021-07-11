
#include <iostream>

#include <cronch/deserialize.hpp>
#include <cronch/json/json.hpp>
#include <cronch/meta.hpp>
#include <cronch/serialize.hpp>
#include <cronch/xml/pugi.hpp>

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

constexpr auto n = cr::meta::name("m");

int main()
{
    std::stringstream ss;
    for (std::string line; std::getline(std::cin, line);) {
        ss << line << '\n';
    }
    std::cout << "json data: (json): " << ss.str() << '\n';
    const auto my_obj = cr::deserialize<my_type>(cr::json{ss.str()});
    std::cout << "my_type (xml): " << cr::serialize<cr::xml::pugi>(my_obj) << '\n'
              << "(with attrs): " << cr::serialize<cr::xml::pugi_attr>(my_obj)
              << '\n';
}
