
#include "cronch/deserialize.hpp"
#include <doctest/doctest.h>

#include <cronch/json/boost.hpp>
#include <cronch/serialize.hpp>
#include <cronch/meta.hpp>

namespace {
struct test_type {
    int v;
    std::vector<std::string> v2;

    auto operator<=>(const test_type&) const noexcept = default;
};
}

CRONCH_META_TYPE(test_type, (cronch::meta::field("v", &test_type::v), cronch::meta::field("v2", &test_type::v2)))



TEST_SUITE("json_boost") {
    TEST_CASE("Deserialize works") {
        const auto expected = boost::json::parse(R"({ "v": 2, "v2": [ "Test", "Me" ] })");
        test_type input{ .v = 2, .v2 = { "Test", "Me" } };
        boost::json::value b;
        cronch::serialize<cronch::json::boost>(input, b);
        CHECK(expected == b);

        const auto output = cronch::deserialize<test_type>(cronch::json::boost{b});
        CHECK(output == input);
    }


}


