#include <cronch/json/boost.hpp>

struct my_value_type {
    int i;
};

template<>
struct cronch::json::boost::converter<my_value_type> {

    void to_json(::boost::json::value& doc, const my_value_type& v) {
        doc.emplace_int64() = v.i;
    }
    void from_json(const ::boost::json::value& doc, my_value_type& v) {
        v.i = doc.as_int64();
    }
};
