
#include <benchmark/benchmark.h>

#include <cronch/deserialize.hpp>
#include <cronch/json/json.hpp>
#include <cronch/serialize.hpp>

#include <nlohmann/json.hpp>

#include "types.hpp"

namespace cronch::bm {
namespace {
auto setup_jbtype()
{
    const auto b = setup_btype();
    nlohmann::json j{{"i", b.i}, {"v", b.v}, {"v2", b.v2}, {"hidden", b.hidden()}};
    return j;
}
static void escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }

static void clobber() { asm volatile("" : : : "memory"); }
} // namespace

static void json_serial_type(benchmark::State& st)
{
    const auto b = setup_btype();
    for (auto _ : st) {
        const auto str = cr::serialize<cr::json::nloh>(b);
        escape((void*)str.data());
        clobber();
    }
}

static void json_ctor(benchmark::State& st) {
    for (auto _ : st) {
        const auto j = setup_jbtype();
        escape((void*)&j);
        clobber();
    }
}
static void json_dump(benchmark::State& st)
{
    const auto j = setup_jbtype();
    for (auto _ : st) {
        const auto str = j.dump();
        escape((void*)str.data());
        clobber();
    }
}
BENCHMARK(json_dump);
BENCHMARK(json_ctor);
BENCHMARK(json_serial_type);

} // namespace bm
