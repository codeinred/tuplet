#include <benchmark/benchmark.h>
#include <cstdint>
#include <tuple>
#include <tuplet/tuple.hpp>
#include <type_traits>

std::vector<std::tuple<int64_t>> std_tuple_v4(4);
std::vector<std::tuple<int64_t>> std_tuple_v8(8);
std::vector<std::tuple<int64_t>> std_tuple_v16(16);
std::vector<std::tuple<int64_t>> std_tuple_v32(32);
std::vector<std::tuple<int64_t>> std_tuple_v64(64);
std::vector<std::tuple<int64_t>> std_tuple_v128(128);
std::vector<tuplet::tuple<int64_t>> tuplet_tuple_v4(4);
std::vector<tuplet::tuple<int64_t>> tuplet_tuple_v8(8);
std::vector<tuplet::tuple<int64_t>> tuplet_tuple_v16(16);
std::vector<tuplet::tuple<int64_t>> tuplet_tuple_v32(32);
std::vector<tuplet::tuple<int64_t>> tuplet_tuple_v64(64);
std::vector<tuplet::tuple<int64_t>> tuplet_tuple_v128(128);
template <class T>
void BM_copy(benchmark::State& state, T value) {
    for (auto _ : state) {
        T dest = value;
        benchmark::DoNotOptimize(dest);
    }
}
BENCHMARK_CAPTURE(BM_copy, "Copy vector std_tuple_v4", std_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, "Copy vector tuplet_tuple_v4", tuplet_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, "Copy vector std_tuple_v8", std_tuple_v8);
BENCHMARK_CAPTURE(BM_copy, "Copy vector tuplet_tuple_v8", tuplet_tuple_v8);
BENCHMARK_CAPTURE(BM_copy, "Copy vector std_tuple_v16", std_tuple_v16);
BENCHMARK_CAPTURE(BM_copy, "Copy vector tuplet_tuple_v16", tuplet_tuple_v16);
BENCHMARK_CAPTURE(BM_copy, "Copy vector std_tuple_v32", std_tuple_v32);
BENCHMARK_CAPTURE(BM_copy, "Copy vector tuplet_tuple_v32", tuplet_tuple_v32);
BENCHMARK_CAPTURE(BM_copy, "Copy vector std_tuple_v64", std_tuple_v64);
BENCHMARK_CAPTURE(BM_copy, "Copy vector tuplet_tuple_v64", tuplet_tuple_v64);
BENCHMARK_CAPTURE(BM_copy, "Copy vector std_tuple_v128", std_tuple_v128);
BENCHMARK_CAPTURE(BM_copy, "Copy vector tuplet_tuple_v128", tuplet_tuple_v128);

BENCHMARK_MAIN();
