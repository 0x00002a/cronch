
#include <doctest/doctest.h>

#include <cronch/json/json.hpp>
#include <cronch/meta.hpp>
#include <cronch/metadata.hpp>
#include <cronch/serialize.hpp>
#include <cronch/xml/pugi.hpp>

#include <string>
#include <vector>

namespace cronch::tests {

struct serializable_mock {
    std::vector<std::string> vstr;
    std::string str;
    int i;
};
}
namespace cronch {
template<>
struct metadata<tests::serializable_mock> {
    static constexpr auto name = "serializable_mock";
    static constexpr auto fields =
        meta::fields(meta::field("vstr", &tests::serializable_mock::vstr),
                     meta::field("str", &tests::serializable_mock::str),
                     meta::field("i", &tests::serializable_mock::i));
};


}
namespace cronch::tests {
TEST_SUITE("serialize")
{

    TEST_CASE("serializing a struct known to cronch results in a string output "
              "in the format specified")
    {
        const auto target = serializable_mock{
            .vstr = {"Hello", "World"}, .str = "testme", .i = 5};
        SUBCASE("JSON")
        {
            const auto expected = nlohmann::json{
                {"vstr", target.vstr}, {"str", target.str}, {"i", target.i}};
            const auto parsed = nlohmann::json::parse(
                cronch::serialize<cronch::json::nloh>(target));
            std::cout << parsed << '\n';
            CHECK(parsed == expected);
        }
    }
}

} // namespace cronch::tests
