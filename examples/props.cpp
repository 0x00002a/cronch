
#include <cronch/serialize.hpp>
#include <cronch/meta.hpp>
#include <cronch/json/json.hpp>

#include <iostream>

class my_type {
public:

    void value(const std::string& v) {
        value_ = v;
    } 

    auto value() const -> const std::string& {
        return value_;
    }
    auto i() const -> int {
        return i_;
    }
    void set_i(int v) {
        i_ = v;
    }
private:
    std::string value_;
    int i_;
};

namespace cm = cronch::meta;



CRONCH_META_TYPE(my_type, (
            cm::property("value", &my_type::value, &my_type::value),
            cm::property("i", &my_type::i, &my_type::set_i)

            ))

int main() {

    my_type t;
    t.set_i(5);
    t.value("Hello");

    std::cout << cronch::serialize<cronch::json::nloh>(t) << '\n';

}


