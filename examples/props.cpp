
#include <cronch/serialize.hpp>
#include <cronch/meta.hpp>
#include <cronch/json/nloh.hpp>

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


void set_v(my_type& t, const std::string& s) {
    t.value(s);
}
auto get_v(const my_type& t) -> const std::string&  {
    return t.value();
}

CRONCH_META_TYPE(my_type, (
            cm::property("value", &get_v, &set_v),
            cm::property("i", &my_type::i, &my_type::set_i)

            ))

int main() {

    my_type t;
    t.set_i(5);
    t.value("Hello");

    std::cout << cronch::serialize<cronch::json::nloh>(t) << '\n';

}


