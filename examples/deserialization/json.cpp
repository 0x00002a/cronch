
#include "cronch/deserialize.hpp"
#include <cronch/serialize.hpp> 
#include <cronch/meta.hpp> 
#include <cronch/metadata.hpp> 
#include <cronch/json/nloh.hpp>

#include <iostream>
#include <vector>


struct my_type {
    std::vector<std::string> some_values;
    std::string a_value;
};

namespace cm = cronch::meta;

CRONCH_META_TYPE(my_type, (
    cm::field("some values", &my_type::some_values),
    cm::field("value", &my_type::a_value)
            ))


static_assert(cronch::concepts::known_to_cronch<my_type>);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Wrong number of arguments, expects 1\n";
        return EXIT_FAILURE;
    }


    std::string input{argv[1]};
    const auto my_obj = cronch::deserialize<my_type>(cronch::json::nloh{input});
    std::cout << "some_values: [ ";
    std::for_each(my_obj.some_values.begin(), my_obj.some_values.end(), [](const auto& s) { std::cout << s << ", ";  });
    std::cout << "]\n" << "a_value: " << my_obj.a_value << '\n';


}


