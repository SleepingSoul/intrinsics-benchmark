#include <intrin.h>
#include <iostream>
#include <array>
#include <limits>

void convert16Uints8To16Float32()
{

}

int main()
{
    __m256i input;


    for (int i = 0; i < 32; ++i)
    {
        input.m256i_i8[i] = 128;
    }

    __m256i zero = _mm256_set1_epi8(0);

    // Convert array of chars into 2 separate arrays of int16 and store in registers
    __m256i r1 = _mm256_unpackhi_epi8(input, zero); // requires AVX2
    __m256i r2 = _mm256_unpacklo_epi8(input, zero);

    // Convert arrays of int16 to arrays of int32 and store in registers
    __m256i r3 = _mm256_cvtepi16_epi32(*(__m128i*) & r1.m256i_u16[0]); // requires AVX2
    __m256i r4 = _mm256_cvtepi16_epi32(*(__m128i*) & r2.m256i_u16[0]);
    __m256i r5 = _mm256_cvtepi16_epi32(*(__m128i*) & r1.m256i_u16[15]);
    __m256i r6 = _mm256_cvtepi16_epi32(*(__m128i*) & r2.m256i_u16[15]);

    // Convert arrays of int32 into arrays of float32 and store in registers
    __m256 f1 = _mm256_cvtepi32_ps(r3); // requires AVX
    __m256 f2 = _mm256_cvtepi32_ps(r4);
    __m256 f3 = _mm256_cvtepi32_ps(r5);
    __m256 f4 = _mm256_cvtepi32_ps(r6);

    __m256 uint8Max = _mm256_set1_ps(255.f);

    // Divide every float32 to normalize value
    __m256 n1 = _mm256_div_ps(f1, uint8Max);
    __m256 n2 = _mm256_div_ps(f2, uint8Max);
    __m256 n3 = _mm256_div_ps(f3, uint8Max);
    __m256 n4 = _mm256_div_ps(f4, uint8Max);

    for (int i = 0; i < 8; ++i)
    {
        std::cout << i << ". " << n1.m256_f32[i] << ' ' << n2.m256_f32[i] << '\n';
    }

    for (int i = 0; i < 8; ++i)
    {
        std::cout << i + 8 << ". " << n3.m256_f32[i] << ' ' << n4.m256_f32[i] << '\n';
    }

    return 0;
}
