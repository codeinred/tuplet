#pragma once
#include <benchmark/benchmark.h>

template <class T>
void BM_copy(benchmark::State& state, T value) {
    T dest;
    for (auto _ : state) {
        dest = value;
        benchmark::DoNotOptimize(dest);
    }
}
