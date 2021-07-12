
#include <iostream>

#include <cronch/deserialize.hpp>
#include <cronch/json/json.hpp>
#include <cronch/meta.hpp>
#include <cronch/metatypes.hpp>
#include <cronch/serialize.hpp>
#include <cronch/xml/pugi.hpp>

#include <string>

namespace cr = cronch;

struct my_type {
    std::string name;
    int some_info;
    std::vector<int> values;
};

template<>
struct cr::metadata<my_type> {
    // clang-format off
    constexpr static auto name = "my_type";
    constexpr static auto fields = cr::meta::fields(
        cr::meta::field("name", &my_type::name),
        cr::meta::field("some_info", &my_type::some_info),
        cr::meta::field("values", &my_type::values)
    );
    // clang-format on
};

constexpr auto n = cr::meta::name("m");

int main(int argc, char** argv)
{

    std::stringstream ss;
    if (argc == 1) {
        std::cerr << "Not enough args\n";
        return EXIT_FAILURE;
    }
    else if (argc < 2) {
        for (std::string line; std::getline(std::cin, line);) {
            ss << line << '\n';
        }
    }
    else {
        ss.write(argv[1], std::strlen(argv[1]));
    }
    std::cout << "json data: (json): " << ss.str() << '\n';
    auto my_obj = cr::deserialize<my_type>(cr::json::nloh{ss.str()});
    std::cout << "my_type (xml): " << cr::serialize<cr::xml::pugi>(my_obj)
              << '\n'
              << "(with attrs): " << cr::serialize<cr::xml::pugi_attr>(my_obj)
              << '\n';
}
