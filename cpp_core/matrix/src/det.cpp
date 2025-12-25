#include "matrix/matrix.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

namespace matrixlib {

// Determinant for small matrices using Gaussian elimination with partial pivoting.
// Restriction: n <= 10
double det_small(const Matrix& a) {
    if (a.rows() != a.cols()) throw std::invalid_argument("det_small: matrix must be square");
    std::size_t n = a.rows();
    if (n > 10) throw std::invalid_argument("det_small: supported only for n <= 10");

    // copy into double for numerical stability
    std::vector<double> m(n * n);
    for (std::size_t i = 0; i < n*n; ++i) m[i] = static_cast<double>(a.vec()[i]);

    auto idx = [n](std::size_t r, std::size_t c) { return r * n + c; };

    int sign = 1;
    double det = 1.0;
    const double eps = 1e-12;

    for (std::size_t k = 0; k < n; ++k) {
        // pivot
        std::size_t piv = k;
        double maxabs = std::fabs(m[idx(k, k)]);
        for (std::size_t i = k + 1; i < n; ++i) {
            double v = std::fabs(m[idx(i, k)]);
            if (v > maxabs) { maxabs = v; piv = i; }
        }
        if (maxabs < eps) return 0.0; // singular

        if (piv != k) {
            // swap rows
            for (std::size_t j = 0; j < n; ++j)
                std::swap(m[idx(k, j)], m[idx(piv, j)]);
            sign *= -1;
        }

        double pivot = m[idx(k, k)];
        det *= pivot;

        // eliminate below
        for (std::size_t i = k + 1; i < n; ++i) {
            double factor = m[idx(i, k)] / pivot;
            m[idx(i, k)] = 0.0;
            for (std::size_t j = k + 1; j < n; ++j) {
                m[idx(i, j)] -= factor * m[idx(k, j)];
            }
        }
    }

    return det * static_cast<double>(sign);
}

} // namespace matrixlib
