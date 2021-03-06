// Benchmarks.cpp - benchmarks for BinaryFormatSerializer
//

#include "serializers/VectorSaveSerializer.h"

#include "formatters/endian_formatter.h"
#include "formatters/vector_formatter.h"

#include "benchmark/benchmark.h"

#include <cstdint>

using namespace binary_format;

static void BM_VectorVerbatim(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    std::vector<int16_t> ints(10'000, -2);
    using vector_verbatim = vector_formatter< little_endian<2>, little_endian<2> >;

    while (state.KeepRunning())
    {
        save< vector_verbatim >(vectorWriter, ints);
        benchmark::DoNotOptimize( vectorWriter.getData() );
    }
}
BENCHMARK(BM_VectorVerbatim);

static void BM_VectorNonVerbatim(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    std::vector<int8_t> ints(10'000, -2);
    using vector_nonverbatim = vector_formatter< little_endian<2>, little_endian<2> >;

    while (state.KeepRunning())
    {
        save< vector_nonverbatim >(vectorWriter, ints);
        benchmark::DoNotOptimize( vectorWriter.getData() );
    }
}
BENCHMARK(BM_VectorNonVerbatim);

static void BM_IntVerbatim(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    int32_t a = 50;
    while (state.KeepRunning())
    {
        save< little_endian<4> >(vectorWriter, a);
    }
    benchmark::DoNotOptimize( vectorWriter.getData() );
}
BENCHMARK(BM_IntVerbatim);

static void BM_IntNonVerbatim(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    struct { int32_t a; } b = { 50 };
    while (state.KeepRunning())
    {
        save< little_endian<4> >(vectorWriter, b);
    }
    benchmark::DoNotOptimize( vectorWriter.getData() );
}
BENCHMARK(BM_IntNonVerbatim);

static void BM_IntBigNonVerbatim(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    struct { int32_t a; } b = { 50 };
    while (state.KeepRunning())
    {
        save< big_endian<4> >(vectorWriter, b);
    }
    benchmark::DoNotOptimize( vectorWriter.getData() );
}
BENCHMARK(BM_IntBigNonVerbatim);

static void BM_IntBig(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    int32_t a = 50;
    while (state.KeepRunning())
    {
        save< big_endian<4> >(vectorWriter, a);
    }
    benchmark::DoNotOptimize( vectorWriter.getData() );
}
BENCHMARK(BM_IntBig);

static void BM_IntVerbatimWiden(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    int16_t a = 50;
    while (state.KeepRunning())
    {
        save< little_endian<4> >(vectorWriter, a);
    }
    benchmark::DoNotOptimize( vectorWriter.getData() );
}
BENCHMARK(BM_IntVerbatimWiden);

static void BM_IntBigWiden(benchmark::State& state) {
    VectorSaveSerializer vectorWriter;

    int16_t a = 50;
    while (state.KeepRunning())
    {
        save< big_endian<4> >(vectorWriter, a);
    }
    benchmark::DoNotOptimize( vectorWriter.getData() );
}
BENCHMARK(BM_IntBigWiden);

BENCHMARK_MAIN();
