#include <benchmark/benchmark.h>
#include <tile_to_merc.hpp>

static void BM_double(benchmark::State& state) // NOLINT google-runtime-references
{
    sse_example::tile_vector tiles;

    for (int x = state.range(0); x > 0; x--)
    {
        tiles.push_back({10, rand() % (1 << 10), rand() % (1 << 10)});
    }

    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(sse_example::box_from_tile(tiles));
    }

    state.counters["Tiles"] = static_cast<double>(tiles.size());
}

BENCHMARK(BM_double)->RangeMultiplier(4)->Range(32, 1 << 14);

static void BM_float(benchmark::State& state) // NOLINT google-runtime-references
{
    sse_example::tile_vector tiles;

    for (int x = state.range(0); x > 0; x--)
    {
        tiles.push_back({10, rand() % (1 << 10), rand() % (1 << 10)});
    }

    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(sse_example::box_from_tile_float(tiles));
    }

    state.counters["Tiles"] = static_cast<double>(tiles.size());
}

BENCHMARK(BM_float)->RangeMultiplier(4)->Range(32, 1 << 14);

static void BM_sse_double(benchmark::State& state) // NOLINT google-runtime-references
{
    sse_example::tile_vector tiles;

    for (int x = state.range(0); x > 0; x--)
    {
        tiles.push_back({10, rand() % (1 << 10), rand() % (1 << 10)});
    }

    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(sse_example::box_from_tile_sse_double(tiles));
    }

    state.counters["Tiles"] = static_cast<double>(tiles.size());
}

BENCHMARK(BM_sse_double)->RangeMultiplier(4)->Range(32, 1 << 14);

static void BM_sse_float(benchmark::State& state) // NOLINT google-runtime-references
{
    sse_example::tile_vector tiles;

    for (int x = state.range(0); x > 0; x--)
    {
        tiles.push_back({10, rand() % (1 << 10), rand() % (1 << 10)});
    }

    while (state.KeepRunning())
    {
        auto out = sse_example::box_from_tile_sse_float(tiles);
        benchmark::DoNotOptimize(out.size());
    }

    state.counters["Tiles"] = static_cast<double>(tiles.size());
}

BENCHMARK(BM_sse_float)->RangeMultiplier(4)->Range(32, 1 << 14);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
BENCHMARK_MAIN();
#pragma GCC diagnostic pop
