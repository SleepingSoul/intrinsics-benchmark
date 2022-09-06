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

        // Store results in array so we can copy into the vector at once in the end
        __m256 normalized[4];

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

#ifndef _DEBUGMODE
        __m256 uint8Max = _mm256_set1_ps(255.f);
#else
        // For debug - I just want to see input numbers converted to floats
        __m256 uint8Max = _mm256_set1_ps(1.f);
#endif

        // Divide every float32 to normalize value
        normalized[0] = _mm256_div_ps(f1, uint8Max); // requires AVX
        normalized[1] = _mm256_div_ps(f2, uint8Max);
        normalized[2] = _mm256_div_ps(f3, uint8Max);
        normalized[3] = _mm256_div_ps(f4, uint8Max);

        // Main bottleneck - copy data into RAM
        std::copy_n(reinterpret_cast<float*>(normalized), 32, output.data());
	}

    void convert32Int16To32Float32(const std::vector<std::int16_t>& input, std::vector<float>& output)
    {
        assert(input.size() == 32 && output.size() == 32);

        // Store results in array so we can copy into the vector at once in the end
        __m256 normalized[4];

        // Preload 32 shorts into 2 registers from RAM
        __m256i loadedData1 = _mm256_loadu_epi16(reinterpret_cast<const void*>(&input[0]));
        __m256i loadedData2 = _mm256_loadu_epi16(reinterpret_cast<const void*>(&input[16]));


        // Convert array of shorts into 4 separate arrays of int32
        __m256i i1 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData1.m256i_u16[0])); // requires AVX2
        __m256i i2 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData1.m256i_u16[8]));
        __m256i i3 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData2.m256i_u16[0]));
        __m256i i4 = _mm256_cvtepi16_epi32(*reinterpret_cast<__m128i*>(&loadedData2.m256i_u16[8]));

        // Convert arrays of int32 into arrays of float32 and store in registers
        __m256 f1 = _mm256_cvtepi32_ps(i1); // requires AVX
        __m256 f2 = _mm256_cvtepi32_ps(i2);
        __m256 f3 = _mm256_cvtepi32_ps(i3);
        __m256 f4 = _mm256_cvtepi32_ps(i4);

#ifndef _DEBUGMODE
        __m256i int16Max = _mm256_set1_epi32(std::numeric_limits<std::int16_t>::max());
#else
        // For debug - I just want to see input numbers converted to floats
        __m256i int16Max = _mm256_set1_epi32(1);
#endif

        // Set sign to divider depending on the sign on values that we have
        __m256i dividersInt32[4] = {
            _mm256_sign_epi32(int16Max, i1),
            _mm256_sign_epi32(int16Max, i2),
            _mm256_sign_epi32(int16Max, i3),
            _mm256_sign_epi32(int16Max, i4)
        };

        // Convert dividers into floats
        __m256 dividersF32[4] = {
            _mm256_cvtepi32_ps(dividersInt32[0]),
            _mm256_cvtepi32_ps(dividersInt32[1]),
            _mm256_cvtepi32_ps(dividersInt32[2]),
            _mm256_cvtepi32_ps(dividersInt32[3])
        };

        // Divide every float32 to normalize value
        normalized[0] = _mm256_div_ps(f1, dividersF32[0]); // requires AVX
        normalized[1] = _mm256_div_ps(f2, dividersF32[1]);
        normalized[2] = _mm256_div_ps(f3, dividersF32[2]);
        normalized[3] = _mm256_div_ps(f4, dividersF32[3]);

        std::copy_n(reinterpret_cast<float*>(normalized), 32, output.data());
    }

    void convert32Int32To32Float32(const std::vector<std::int32_t>& input, std::vector<float>& output)
    {
        assert(input.size() == 32 && output.size() == 32);

        // Store results in array so we can copy into the vector at once in the end
        __m256 normalized[4];

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

#ifndef _DEBUGMODE
        __m256i int32Max = _mm256_set1_epi32(std::numeric_limits<std::int32_t>::max());
#else
        // For debug - I just want to see input numbers converted to floats
        __m256i int32Max = _mm256_set1_epi32(1);
#endif
        __m256 dividersF32[4];

        // Using scope to unwind 4x __m256i values from stack potentially freeing registers
        {
            // Set sign to divider depending on the sign on values that we have
            __m256i dividersInt32[4] = {
                _mm256_sign_epi32(int32Max, loadedData[0]),
                _mm256_sign_epi32(int32Max, loadedData[1]),
                _mm256_sign_epi32(int32Max, loadedData[2]),
                _mm256_sign_epi32(int32Max, loadedData[3])
            };

            // Convert dividers into floats
            dividersF32[0] = _mm256_cvtepi32_ps(dividersInt32[0]);
            dividersF32[1] = _mm256_cvtepi32_ps(dividersInt32[1]);
            dividersF32[2] = _mm256_cvtepi32_ps(dividersInt32[2]);
            dividersF32[3] = _mm256_cvtepi32_ps(dividersInt32[3]);
        }

        // Divide every float32 to normalize value
        normalized[0] = _mm256_div_ps(f1, dividersF32[0]); // requires AVX
        normalized[1] = _mm256_div_ps(f2, dividersF32[1]);
        normalized[2] = _mm256_div_ps(f3, dividersF32[2]);
        normalized[3] = _mm256_div_ps(f4, dividersF32[3]);

        std::copy_n(reinterpret_cast<float*>(normalized), 32, output.data());
    }
}
