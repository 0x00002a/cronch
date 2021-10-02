
#include "cronch/deserialize.hpp"
#include <doctest/doctest.h>

#include <cronch/json/boost.hpp>
#include <cronch/serialize.hpp>
#include <cronch/meta.hpp>

namespace {
struct test_type {
    int v;
    std::vector<std::string> v2;
    float v3;

    auto operator<=>(const test_type&) const noexcept = default;
};
auto operator<<(std::ostream& s, const std::vector<std::string>& v) -> std::ostream&
{
    s << "[ ";
    for (const auto& vs : v) {
        s << vs << ", ";
    }
    s << "]";
    return s;
}
auto operator<<(std::ostream& s, const test_type& t) -> std::ostream&
{
    s << "v: " << t.v << ", v2: " << t.v2 << ", v3: " << t.v3;
    return s;
}
}



CRONCH_META_TYPE(test_type, (cronch::meta::field("v", &test_type::v), cronch::meta::field("v2", &test_type::v2),
                             cronch::meta::field("v3", &test_type::v3)))

TEST_SUITE("json_boost") {
    TEST_CASE("Deserialize works") {
        const auto expected = boost::json::parse(R"({ "v": 2, "v2": [ "Test", "Me" ], "v3": 0.5 })");
        test_type input{.v = 2, .v2 = {"Test", "Me"}, .v3 = 0.5};
        boost::json::value b;
        cronch::serialize<cronch::json::boost>(input, b);
        CHECK(expected == b);

        const auto output = cronch::deserialize<test_type>(cronch::json::boost{b});
        CHECK(output == input);
    }


}


