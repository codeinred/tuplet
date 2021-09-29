#include <benchmark/benchmark.h>
#include <cstdint>
#include <tuple>
#include <tuplet/tuple.hpp>
#include <type_traits>

#include "shared.hpp"

using homo_std_tuple_t = std::tuple<int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t>;
using homo_tuplet_tuple_t = tuplet::tuple<int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t>;

static_assert(sizeof(homo_std_tuple_t) == 8, "Expected std::tuple to be 8 bytes");
static_assert(sizeof(homo_tuplet_tuple_t) == 8, "Expected tuplet::tuple to be 8 bytes");

std::vector<homo_std_tuple_t> homogenous_std_tuple_v4(4);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v8(8);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v16(16);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v32(32);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v64(64);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v128(128);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v256(256);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v512(512);
std::vector<homo_std_tuple_t> homogenous_std_tuple_v1024(1024);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v4(4);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v8(8);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v16(16);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v32(32);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v64(64);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v128(128);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v256(256);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v512(512);
std::vector<homo_tuplet_tuple_t> homogenous_tuplet_tuple_v1024(1024);

BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v4, homogenous_std_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v4, homogenous_tuplet_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v8, homogenous_std_tuple_v8);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v8, homogenous_tuplet_tuple_v8);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v16, homogenous_std_tuple_v16);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v16, homogenous_tuplet_tuple_v16);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v32, homogenous_std_tuple_v32);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v32, homogenous_tuplet_tuple_v32);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v64, homogenous_std_tuple_v64);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v64, homogenous_tuplet_tuple_v64);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v128, homogenous_std_tuple_v128);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v128, homogenous_tuplet_tuple_v128);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v256, homogenous_std_tuple_v256);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v256, homogenous_tuplet_tuple_v256);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v512, homogenous_std_tuple_v512);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v512, homogenous_tuplet_tuple_v512);
BENCHMARK_CAPTURE(BM_copy, homogenous_std_tuple_v1024, homogenous_std_tuple_v1024);
BENCHMARK_CAPTURE(BM_copy, homogenous_tuplet_tuple_v1024, homogenous_tuplet_tuple_v1024);

#ifndef INCLUDE_SUB_BENCHMARKS
BENCHMARK_MAIN();
#endif
