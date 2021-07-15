
#include <cronch/json/nloh.hpp>
#include <cronch/deserialize.hpp>
#include <cronch/serialize.hpp>
#include <cronch/meta.hpp>

#include <iostream>

#include <doctest/doctest.h>

struct other_val {
    int v;

    auto operator<=>(const other_val&) const noexcept = default;
};

struct test_type {
    int value;
    std::vector<other_val> also_values;

    auto operator<=>(const test_type&) const noexcept = default;
};

CRONCH_META_TYPE(test_type, (cronch::meta::field("value", &test_type::value), cronch::meta::field("also", &test_type::also_values)))
CRONCH_META_TYPE(other_val, (cronch::meta::field("v", &other_val::v)))

auto operator<<(std::ostream& os, const std::vector<test_type>& v) {
    os << "[ ";
    for(const auto& t : v) {
        os << "{ v: " << t.value << " }" << ", ";
    }
    os << "] ";
}

TEST_SUITE("json_nloh - deserialize") {

    TEST_CASE("Deserialize with a vector of types unknown to nlohmann/json compiles") {
        nlohmann::json expected = nlohmann::json::parse(R"([ { "value": 2, "also": [ { "v": 4 } ] }, { "value": 4, "also": [ { "v": 6 } ] } ])");
        std::vector<test_type> input;
        for (const auto& v : expected) {
            test_type t{v.at("value").get<int>()};
            for (const auto& jalso : v.at("also")) {
                t.also_values.emplace_back(other_val{jalso.at("v").get<int>()});
            }
            input.emplace_back(std::move(t));
        }

        auto actual = cronch::deserialize<std::vector<test_type>>(cronch::json::nloh{expected});
        REQUIRE(actual == input);
}

TEST_SUITE("json_nloh - serialize") {
    TEST_CASE("Serialize with a type containing a vector of types unknown to nlohmann/json but known to cronch compiles and provides expected json") {
        nlohmann::json expected = nlohmann::json::parse(R"( { "value": 2, "also": [ { "v": 15 } ] } )");
        nlohmann::json actual;
        test_type input{2, {{15}}};
        cronch::serialize<cronch::json::nloh>(input, actual);
        REQUIRE(actual == expected);
    }
}

}

