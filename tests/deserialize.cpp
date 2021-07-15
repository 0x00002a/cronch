
#include <cronch/deserialize.hpp>
#include <cronch/meta.hpp>
#include <cronch/json/nloh.hpp>

#include <iostream>

#include <doctest/doctest.h>

struct other_val {

};

struct test_type {
    int value;
    std::vector<other_val> also_values;

    auto operator<=>(const test_type&) const noexcept = default;
};

CRONCH_META_TYPE(test_type, (cronch::meta::field("value", &test_type::value), cronch::meta::field("also", &test_type::also_values)))
CRONCH_META_TYPE(other_val)

auto operator<<(std::ostream& os, const std::vector<test_type>& v) {
    os << "[ ";
    for(const auto& t : v) {
        os << "{ v: " << t.value << " }" << ", ";
    }
    os << "] ";
}

TEST_SUITE("deserialize") {

    TEST_CASE("Deserialize with a vector of types unknown to nlohmann/json compiles") {
        nlohmann::json expected = nlohmann::json::parse(R"([ { "value": 2, "also": [] }, { "value": 4, "also": [] } ])");
        std::vector<test_type> input;
        for (const auto& v : expected) {
            input.emplace_back(test_type{v.at("value").get<int>()});
        }

        auto actual = cronch::deserialize<std::vector<test_type>>(cronch::json::nloh{expected});
        REQUIRE(actual == input);
}

}

