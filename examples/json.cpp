
#include <cronch/ast.hpp>

#include <string>

namespace cronch = cr;

struct my_type {
    std::string name;
    int some_info;
};

template<>
struct cr::metadata<my_type> {
    constexpr static auto about =
        cr::meta::info(cr::meta::name("my_type"),
                       cr::meta::field("name", &my_type::name),
                       cr::meta::field("some_info", &my_type::some_info));
};

int main()
{
    std::cout << "my_type: "
              << cr::serialize<cr::json>(my_type{"Hello there", 4}) << '\n';
}
