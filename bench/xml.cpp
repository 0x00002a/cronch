
#include "types.hpp"

#include <cronch/serialize.hpp> 
#include <cronch/xml/pugi.hpp> 

#include <benchmark/benchmark.h> 
#include <sstream>

#include <cronch/metatypes.hpp>


namespace cronch::bm {

    namespace {
        auto setup_bxml(const bench_type& b) {

            pugi::xml_document doc;
            auto node = doc.append_child(meta::nameof<bench_type>());
            node.append_child("v2").append_child(pugi::node_pcdata).set_value(b.v2.c_str());
            node.append_child("i").append_child(pugi::node_pcdata).set_value(boost::lexical_cast<std::string>(b.i).c_str());
            auto vnode = node.append_child("v");
            for (const auto& val : b.v) {
                vnode.append_child("std::string").append_child(pugi::node_pcdata).set_value(val.c_str());
            }
            node.append_child("hidden").append_child(pugi::node_pcdata).set_value(b.hidden().c_str());

            return doc;
        }
    }


    static void pugi_ctor(benchmark::State& st) {
        const auto b = setup_btype();
        for (auto _ : st) {
            setup_bxml(b);
        }
    }
    static void pugi_serial(benchmark::State& st) {
        const auto xml = setup_bxml(setup_btype());
        for (auto _ : st) {
            std::stringstream ss;
            xml.save(ss);
        }
    }
    static void pugi_serial_type(benchmark::State& st) {
        const auto b = setup_btype();
        for (auto _ : st) {
            serialize<xml::pugi>(b);
        }
    }

    BENCHMARK(pugi_serial);
    BENCHMARK(pugi_serial_type);
    BENCHMARK(pugi_ctor);


}

