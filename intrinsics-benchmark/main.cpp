#include <intrin.h>
#include <iostream>
#include <vector>
#include <limits>
#include <ctime>
#include <random>

#include <benchmark/benchmark.h>
#pragma comment(lib, "shlwapi.lib")


static void BM_Convert32FCharsToFloat32_Usual(benchmark::State& state)
{
    std::vector<std::uint8_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::uint8_t>(rand()); });

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            result[i] = static_cast<float>(data[i]) / 255.f;
        }
    }
}
BENCHMARK(BM_Convert32FCharsToFloat32_Usual);


static void BM_Convert32FCharsToFloat32_Intrinsics(benchmark::State& state)
{
    std::vector<std::uint8_t> data(32, 0);
    std::vector<float> result(32, 0);

    std::generate(data.begin(), data.end(), [] { return static_cast<std::uint8_t>(rand()); });

    for (auto _ : state)
    {
        // Store results in array so we can copy into the vector at once in the end
        __m256 normalized[4];

        // Preload 32 chars into register from RAM
        __m256i loadedData = _mm256_loadu_epi8(data.data());

        // Convert array of chars into 4 separate arrays of int32
        __m256i i1 = _mm256_cvtepi8_epi32(*reinterpret_cast<__m128i*>(&loadedData.m256i_u8[0])); // requires AVX2
        __m256i i2 = _mm256_cvtepi8_epi32(*reinterpret_cast<__m128i*>(&loadedData.m256i_u8[8]));
        __m256i i3 = _mm256_cvtepi8_epi32(*reinterpret_cast<__m128i*>(&loadedData.m256i_u8[16]));
        __m256i i4 = _mm256_cvtepi8_epi32(*reinterpret_cast<__m128i*>(&loadedData.m256i_u8[24]));

        // Convert arrays of int32 into arrays of float32 and store in registers
        __m256 f1 = _mm256_cvtepi32_ps(i1); // requires AVX
        __m256 f2 = _mm256_cvtepi32_ps(i2);
        __m256 f3 = _mm256_cvtepi32_ps(i3);
        __m256 f4 = _mm256_cvtepi32_ps(i4);

        __m256 uint8Max = _mm256_set1_ps(255.f);

        // Divide every float32 to normalize value
        normalized[0] = _mm256_div_ps(f1, uint8Max); // requires AVX
        normalized[1] = _mm256_div_ps(f2, uint8Max);
        normalized[2] = _mm256_div_ps(f3, uint8Max);
        normalized[3] = _mm256_div_ps(f4, uint8Max);

        std::copy_n(reinterpret_cast<float*>(normalized), 16, result.data());
    }
}
BENCHMARK(BM_Convert32FCharsToFloat32_Intrinsics);

BENCHMARK_MAIN();
