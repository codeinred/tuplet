#include <benchmark/benchmark.h>
#include <cstdint>
#include <tuple>
#include <tuplet/tuple.hpp>
#include <type_traits>

#include "shared.hpp"

#include <tao/tuple/tuple.hpp>

using hetero_std_tuple_t = std::tuple<int8_t, int8_t, int16_t, int32_t>;
using hetero_tao_tuple_t = tao::tuple<int8_t, int8_t, int16_t, int32_t>;
using hetero_tuplet_tuple_t = tuplet::tuple<int8_t, int8_t, int16_t, int32_t>;

static_assert(std::is_trivially_copyable_v<hetero_tao_tuple_t>);
// For some reason this doesn't apply in windows
#ifndef _MSC_VER
static_assert(sizeof(hetero_std_tuple_t) == 8, "Expected std::tuple to be 8 bytes");
#endif
static_assert(sizeof(hetero_tuplet_tuple_t) == 8, "Expected tuplet::tuple to be 8 bytes");

std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v4(4);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v8(8);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v16(16);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v32(32);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v64(64);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v128(128);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v256(256);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v512(512);
std::vector<hetero_std_tuple_t> heterogenous_std_tuple_v1024(1024);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v4(4);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v8(8);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v16(16);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v32(32);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v64(64);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v128(128);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v256(256);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v512(512);
std::vector<hetero_tao_tuple_t> heterogenous_tao_tuple_v1024(1024);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v4(4);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v8(8);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v16(16);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v32(32);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v64(64);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v128(128);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v256(256);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v512(512);
std::vector<hetero_tuplet_tuple_t> heterogenous_tuplet_tuple_v1024(1024);


BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v4, heterogenous_std_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v4, heterogenous_tao_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v4, heterogenous_tuplet_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v8, heterogenous_std_tuple_v8);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v8, heterogenous_tao_tuple_v8);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v8, heterogenous_tuplet_tuple_v8);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v16, heterogenous_std_tuple_v16);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v16, heterogenous_tao_tuple_v16);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v16, heterogenous_tuplet_tuple_v16);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v32, heterogenous_std_tuple_v32);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v32, heterogenous_tao_tuple_v32);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v32, heterogenous_tuplet_tuple_v32);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v64, heterogenous_std_tuple_v64);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v64, heterogenous_tao_tuple_v64);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v64, heterogenous_tuplet_tuple_v64);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v128, heterogenous_std_tuple_v128);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v128, heterogenous_tao_tuple_v128);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v128, heterogenous_tuplet_tuple_v128);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v256, heterogenous_std_tuple_v256);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v256, heterogenous_tao_tuple_v256);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v256, heterogenous_tuplet_tuple_v256);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v512, heterogenous_std_tuple_v512);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v512, heterogenous_tao_tuple_v512);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v512, heterogenous_tuplet_tuple_v512);
BENCHMARK_CAPTURE(BM_copy, heterogenous_std_tuple_v1024, heterogenous_std_tuple_v1024);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tao_tuple_v1024, heterogenous_tao_tuple_v1024);
BENCHMARK_CAPTURE(BM_copy, heterogenous_tuplet_tuple_v1024, heterogenous_tuplet_tuple_v1024);

#ifndef INCLUDE_SUB_BENCHMARKS
BENCHMARK_MAIN();
#endif
