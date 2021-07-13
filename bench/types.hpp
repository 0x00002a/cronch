
#pragma once

#include <string>
#include <vector>

#include <cronch/meta.hpp>
#include <cronch/metadata.hpp>

namespace cronch::bm {
struct bench_type {
    int i;
    std::vector<std::string> v;
    std::string v2;

    std::string hidden_;

    void hidden(const std::string& hv) { hidden_ = hv; }
    constexpr auto hidden() const -> const std::string& { return hidden_; }
};

inline auto setup_btype()
{
    bench_type b;
    b.i = 4;
    b.v2 = "Nope";
    b.v = std::vector<std::string>{"Hello", "I am", "Being", "Benchmarked"};
    b.hidden("Can't find me");
    return b;
}
} // namespace cronch::bm
using cronch::bm::bench_type;
namespace cr = cronch;

CRONCH_META_TYPE(bench_type, (cr::meta::field("i", &bench_type::i), cr::meta::field("v", &bench_type::v),
                              cr::meta::field("v2", &bench_type::v2),
                              cr::meta::property("hidden", &bench_type::hidden, &bench_type::hidden)))
