#include <random>
#include <iostream>
#include <iomanip>

#include <benchmark/benchmark.h>
#pragma comment(lib, "shlwapi.lib")

#include "intrinsics_converts.h"

static void BM_Convert32CharsToFloat32_Usual(benchmark::State& state)
{
    std::vector<std::uint8_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::uint8_t>(rand()); });

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            result[i] = static_cast<float>(data[i]) / std::numeric_limits<std::uint8_t>::max();
        }
    }
}

static void BM_Convert32CharsToFloat32_Intrinsics(benchmark::State& state)
{
    std::vector<std::uint8_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::uint8_t>(rand()); });

    for (auto _ : state)
    {
        convert32Uint8To32Float32(data, result);
    }
}

static void BM_Convert32Int16ToFloat32_Usual(benchmark::State& state)
{
    std::vector<std::int16_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::int16_t>(rand()); });

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            result[i] = static_cast<float>(data[i]) / std::numeric_limits<std::int16_t>::max();
        }
    }
}

static void BM_Convert32Int16ToFloat32_Intrinsics(benchmark::State& state)
{
    std::vector<std::int16_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::int16_t>(rand()); });

    for (auto _ : state)
    {
        convert32Int16To32Float32(data, result);
    }
}

static void BM_Convert32Int32ToFloat32_Usual(benchmark::State& state)
{
    std::vector<std::int32_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::int32_t>(rand()); });

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            result[i] = static_cast<float>(data[i]) / std::numeric_limits<std::int32_t>::max();
        }
    }
}

static void BM_Convert32Int32ToFloat32_Intrinsics(benchmark::State& state)
{
    std::vector<std::int32_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::int32_t>(rand()); });

    for (auto _ : state)
    {
        convert32Int32To32Float32(data, result);
    }
}

#ifndef _DEBUGMODE
BENCHMARK(BM_Convert32CharsToFloat32_Usual);
BENCHMARK(BM_Convert32CharsToFloat32_Intrinsics);
BENCHMARK(BM_Convert32Int16ToFloat32_Usual);
BENCHMARK(BM_Convert32Int16ToFloat32_Intrinsics);
BENCHMARK(BM_Convert32Int32ToFloat32_Usual);
BENCHMARK(BM_Convert32Int32ToFloat32_Intrinsics);

BENCHMARK_MAIN();
#else
int main()
{
    // Playground for debug mode
    {
        std::vector<std::uint8_t> data(32, 0);
        std::vector<float> result(32, 0);

        for (std::uint8_t i = 0; i < 32; ++i)
        {
            data[i] = 127;
        }

        convert32Uint8To32Float32(data, result);

        std::cout << "Result of uint8 to float32 conversion:\n";
        for (size_t i = 0; i < 32; ++i)
        {
            std::cout << result[i] << ' ';
        }
        std::cout << '\n';
    }
    {
        std::vector<std::int16_t> data(32, 0);
        std::vector<float> result(32, 0);

        for (std::int16_t i = 0; i < 32; ++i)
        {
            data[i] = i % 2 == 0? 16383 : -16383;
        }

        convert32Int16To32Float32(data, result);

        std::cout << "Result of int16 to float32 conversion:\n";
        for (size_t i = 0; i < 32; ++i)
        {
            std::cout << result[i] << ' ';
        }
        std::cout << '\n';
    }
    {
        std::vector<std::int32_t> data(32, 0);
        std::vector<float> result(32, 0);

        for (std::int32_t i = 0; i < 32; ++i)
        {
            data[i] = i % 2 == 0 ? 1073741823 : -1073741823;
        }

        convert32Int32To32Float32(data, result);

        std::cout << "Result of int32 to float32 conversion:\n";
        for (size_t i = 0; i < 32; ++i)
        {
            std::cout << std::setprecision(10) << std::fixed << result[i] << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
#endif
