
#include <cronch/meta.hpp>

struct my_type {
    int i;
};

CRONCH_META_TYPE(my_type, (cronch::meta::field("i", &my_type::i)))
