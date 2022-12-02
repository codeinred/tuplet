#include <benchmark/benchmark.h>
#include <cstdint>
#include <tuple>
#include <tuplet/tuple.hpp>
#include <type_traits>

#include "shared.hpp"

std::vector<std::tuple<int64_t>> single_elem_std_tuple_v4(4);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v8(8);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v16(16);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v32(32);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v64(64);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v128(128);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v256(256);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v512(512);
std::vector<std::tuple<int64_t>> single_elem_std_tuple_v1024(1024);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v4(4);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v8(8);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v16(16);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v32(32);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v64(64);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v128(128);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v256(256);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v512(512);
std::vector<tuplet::tuple<int64_t>> single_elem_tuplet_tuple_v1024(1024);

BENCHMARK_CAPTURE(BM_copy, single_elem_std_tuple_v4, single_elem_std_tuple_v4);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v4,
    single_elem_tuplet_tuple_v4);
BENCHMARK_CAPTURE(BM_copy, single_elem_std_tuple_v8, single_elem_std_tuple_v8);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v8,
    single_elem_tuplet_tuple_v8);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_std_tuple_v16,
    single_elem_std_tuple_v16);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v16,
    single_elem_tuplet_tuple_v16);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_std_tuple_v32,
    single_elem_std_tuple_v32);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v32,
    single_elem_tuplet_tuple_v32);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_std_tuple_v64,
    single_elem_std_tuple_v64);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v64,
    single_elem_tuplet_tuple_v64);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_std_tuple_v128,
    single_elem_std_tuple_v128);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v128,
    single_elem_tuplet_tuple_v128);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_std_tuple_v256,
    single_elem_std_tuple_v256);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v256,
    single_elem_tuplet_tuple_v256);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_std_tuple_v512,
    single_elem_std_tuple_v512);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v512,
    single_elem_tuplet_tuple_v512);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_std_tuple_v1024,
    single_elem_std_tuple_v1024);
BENCHMARK_CAPTURE(
    BM_copy,
    single_elem_tuplet_tuple_v1024,
    single_elem_tuplet_tuple_v1024);
