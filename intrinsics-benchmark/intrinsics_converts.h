#pragma once

#include <intrin.h>
#include <cstdint>
#include <cassert>
#include <vector>
#include <limits>

namespace
{
    void convert32Uint8To32Float32(const std::vector<std::uint8_t>& input, std::vector<float>& output)
    {
        assert(input.size() == 32 && output.size() == 32);

        // Preload 32 chars into register from RAM
        __m256i loadedData = _mm256_loadu_epi8(input.data());

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

        // This value equals (1 / int16_max) - this way we will avoid using expensive div operation
        __m256 multiplier = _mm256_set1_ps(0.003921568859368562698364257812f);

        __m256 normalized[4] = {
            _mm256_mul_ps(f1, multiplier), // requires AVX
            _mm256_mul_ps(f2, multiplier),
            _mm256_mul_ps(f3, multiplier),
            _mm256_mul_ps(f4, multiplier)
        };

        // Main bottleneck - copy data into RAM
        std::copy_n(reinterpret_cast<float*>(normalized), 32, output.data());
    }

    void convert32Int16To32Float32(const std::vector<std::int16_t>& input, std::vector<float>& output)
    {
        assert(input.size() == 32 && output.size() == 32);

        // Preload 32 shorts into 2 registers from RAM
        __m256i loadedData[2] = {
            _mm256_loadu_epi16(reinterpret_cast<const void*>(&input[0])),
            _mm256_loadu_epi16(reinterpret_cast<const void*>(&input[16]))
        };

        // Convert array of shorts into 4 separate arrays of int32
        __m256i i1 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData[0].m256i_u16[0])); // requires AVX2
        __m256i i2 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData[0].m256i_u16[8]));
        __m256i i3 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData[1].m256i_u16[0]));
        __m256i i4 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData[1].m256i_u16[8]));

        // Convert arrays of int32 into arrays of float32 and store in registers
        __m256 f1 = _mm256_cvtepi32_ps(i1); // requires AVX
        __m256 f2 = _mm256_cvtepi32_ps(i2);
        __m256 f3 = _mm256_cvtepi32_ps(i3);
        __m256 f4 = _mm256_cvtepi32_ps(i4);

        // This value equals (1 / int16_max) - this way we will avoid using expensive div operation
        __m256 multiplier = _mm256_set1_ps(0.00003051850944757462f);

        __m256 normalized[4] = {
            _mm256_mul_ps(f1, multiplier), // requires AVX
            _mm256_mul_ps(f2, multiplier),
            _mm256_mul_ps(f3, multiplier),
            _mm256_mul_ps(f4, multiplier)
        };

        std::copy_n(reinterpret_cast<float*>(normalized), 32, output.data());
    }

    void convert32Int32To32Float32(const std::vector<std::int32_t>& input, std::vector<float>& output)
    {
        assert(input.size() == 32 && output.size() == 32);

        // Preload 32 shorts into 2 registers from RAM
        __m256i loadedData[4] = {
            _mm256_loadu_epi32(reinterpret_cast<const void*>(&input[0])),
            _mm256_loadu_epi32(reinterpret_cast<const void*>(&input[8])),
            _mm256_loadu_epi32(reinterpret_cast<const void*>(&input[16])),
            _mm256_loadu_epi32(reinterpret_cast<const void*>(&input[24])),
        };

        // Convert arrays of int32 into arrays of float32 and store in registers
        __m256 f1 = _mm256_cvtepi32_ps(loadedData[0]); // requires AVX
        __m256 f2 = _mm256_cvtepi32_ps(loadedData[1]);
        __m256 f3 = _mm256_cvtepi32_ps(loadedData[2]);
        __m256 f4 = _mm256_cvtepi32_ps(loadedData[3]);

        // This value equals (1 / int32_max) - this way we will avoid using expensive div operation
        __m256 multiplier = _mm256_set1_ps(0.000000000465661287307739257812f);

        __m256 normalized[4] = {
            _mm256_mul_ps(f1, multiplier), // requires AVX
            _mm256_mul_ps(f2, multiplier),
            _mm256_mul_ps(f3, multiplier),
            _mm256_mul_ps(f4, multiplier)
        };

        std::copy_n(reinterpret_cast<float*>(normalized), 32, output.data());
    }
}
