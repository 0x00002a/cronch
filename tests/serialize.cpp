
#include <doctest/doctest.h>

#include <cronch/json/json.hpp>
#include <cronch/meta.hpp>
#include <cronch/metadata.hpp>
#include <cronch/metatypes.hpp>
#include <cronch/serialize.hpp>
#include <cronch/xml/pugi.hpp>

#include <iostream>

#include <string>
#include <vector>

namespace pugi {

auto operator<<(std::ostream& s, const pugi::xml_document& n) -> std::ostream&
{
    n.print(s);
    return s;
}

} // namespace pugi

namespace cronch::tests {

struct serializable_mock {
    std::vector<std::string> vstr;
    std::string str;
    int i;

    auto hidden() const -> std::string {
        return hidden_;
    }
    void hidden(const std::string& h) {
        hidden_ = h;
    }
    

    std::string hidden_;
};
} // namespace cronch::tests
namespace cronch {
template<>
struct metadata<tests::serializable_mock> {
    static constexpr auto name = "serializable_mock";
    static constexpr auto fields =
        meta::fields(meta::field("vstr", &tests::serializable_mock::vstr),
                     meta::field("str", &tests::serializable_mock::str),
                     meta::field("i", &tests::serializable_mock::i),
                     meta::property("hidden", &tests::serializable_mock::hidden, &tests::serializable_mock::hidden)
                     );
};

static_assert(meta::concepts::accessor<meta::field<tests::serializable_mock, int>>, "field must be an accessor");
static_assert(meta::is_field<meta::field<tests::serializable_mock, int>>::value, "field must be an accessor");
static_assert(!meta::accessors<tests::serializable_mock>().empty(), "field must have accessors");
} // namespace cronch
namespace cronch::tests {
TEST_SUITE("serialize")
{

    TEST_CASE("serializing a struct known to cronch results in a string output "
              "in the format specified")
    {
        const auto target = serializable_mock{
            .vstr = {"Hello", "World"}, .str = "testme", .i = 5, .hidden_ = "cantfindme"};
        SUBCASE("JSON")
        {
            const auto expected = nlohmann::json{
                {"vstr", target.vstr}, {"str", target.str}, {"i", target.i}, {"hidden", target.hidden()}};
            const auto parsed = nlohmann::json::parse(
                cronch::serialize<cronch::json::nloh>(target));
            CHECK(parsed == expected);
        }
        SUBCASE("XML")
        {
            const auto expected = [&] {
                pugi::xml_document doc;
                auto node = doc.append_child(metadata<serializable_mock>::name);
                auto vnode = node.append_child("vstr");
                for (const auto& v : target.vstr) {
                    vnode
                        .append_child(metadata<std::decay_t<decltype(v)>>::name)
                        .append_child(pugi::node_pcdata)
                        .set_value(v.c_str());
                }
                node.append_child("str")
                    .append_child(pugi::node_pcdata)
                    .set_value(target.str.c_str());
                node.append_child("i")
                    .append_child(pugi::node_pcdata)
                    .set_value(
                        boost::lexical_cast<std::string>(target.i).c_str());
                node.append_child("hidden").append_child(pugi::node_pcdata).set_value(target.hidden().c_str());

                return doc;
            }();
            const pugi::xml_document actual = [&] {
                pugi::xml_document doc;
                doc.load_string(
                    cronch::serialize<cronch::xml::pugi>(target).c_str());
                return doc;
            }();
            CHECK(boost::lexical_cast<std::string>(actual) ==
                  boost::lexical_cast<std::string>(expected));
        }
    }
}

} // namespace cronch::tests
