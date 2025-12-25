#include "matrix/matrix.hpp"
#include <algorithm>

namespace matrixlib {

// Task 1: High-level version (naive, but cache-friendly loop order i-k-j)
void matrix_multiply_high_level(const float* A, const float* B, float* C, int n) {
    std::fill(C, C + n * n, 0.0f);
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            float aik = A[i * n + k];
            for (int j = 0; j < n; ++j) {
                C[i * n + j] += aik * B[k * n + j];
            }
        }
    }
}

} // namespace matrixlib
