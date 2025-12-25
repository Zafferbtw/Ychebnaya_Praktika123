#include "matrix/matrix.hpp"

#if defined(__AVX2__) || defined(__AVX__)
  #include <immintrin.h>
#endif

#include <algorithm>

namespace matrixlib {

// Task 1: Low-level version using SIMD (AVX/AVX2).
// Contract: n is expected to be divisible by 8 for full AVX throughput.
// Fallback scalar loop handles tail safely.
void matrix_multiply_low_level(const float* A, const float* B, float* C, int n) {
    std::fill(C, C + n * n, 0.0f);

#if defined(__AVX2__) || defined(__AVX__)
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            __m256 a = _mm256_set1_ps(A[i * n + k]); // broadcast A(i,k)
            int j = 0;
            for (; j + 8 <= n; j += 8) {
                __m256 b = _mm256_loadu_ps(&B[k * n + j]);
                __m256 c = _mm256_loadu_ps(&C[i * n + j]);
                #if defined(__FMA__)
                  c = _mm256_fmadd_ps(a, b, c);
                #else
                  c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
                #endif
                _mm256_storeu_ps(&C[i * n + j], c);
            }
            // tail
            for (; j < n; ++j) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
#else
    // If AVX is not available, fallback to high-level
    matrix_multiply_high_level(A, B, C, n);
#endif
}

} // namespace matrixlib
