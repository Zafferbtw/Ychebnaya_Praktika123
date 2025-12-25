#include "matrix/matrix.hpp"
#include <cmath>
#include <algorithm>

namespace matrixlib {

LUResult lu_decompose(const Matrix& a) {
    if (a.rows() != a.cols()) throw std::invalid_argument("lu_decompose: matrix must be square");
    const int n = static_cast<int>(a.rows());
    LUResult res{a, std::vector<int>(n), 1};

    for (int i = 0; i < n; ++i) res.piv[i] = i;

    const double eps = 1e-12;

    // In-place LU with partial pivoting (Doolittle)
    for (int k = 0; k < n; ++k) {
        // pivot selection
        int piv = k;
        double maxabs = std::fabs(static_cast<double>(res.lu(k, k)));
        for (int i = k + 1; i < n; ++i) {
            double v = std::fabs(static_cast<double>(res.lu(i, k)));
            if (v > maxabs) { maxabs = v; piv = i; }
        }
        if (maxabs < eps) throw std::runtime_error("lu_decompose: singular matrix");

        // swap rows
        if (piv != k) {
            for (int j = 0; j < n; ++j) std::swap(res.lu.vec()[k*n + j], res.lu.vec()[piv*n + j]);
            std::swap(res.piv[k], res.piv[piv]);
            res.piv_sign *= -1;
        }

        // compute multipliers and eliminate
        float pivot = res.lu(k, k);
        for (int i = k + 1; i < n; ++i) {
            res.lu(i, k) = res.lu(i, k) / pivot; // L factor
            float lik = res.lu(i, k);
            for (int j = k + 1; j < n; ++j) {
                res.lu(i, j) = res.lu(i, j) - lik * res.lu(k, j);
            }
        }
    }

    return res;
}

Matrix lu_solve(const LUResult& lu_result, const Matrix& b) {
    const Matrix& lu = lu_result.lu;
    if (lu.rows() != lu.cols()) throw std::invalid_argument("lu_solve: lu must be square");
    const int n = static_cast<int>(lu.rows());
    if (b.rows() != static_cast<std::size_t>(n) || b.cols() != 1) {
        throw std::invalid_argument("lu_solve: b must be (n x 1)");
    }

    // Apply permutation to b: pb
    Matrix x(n, 1);
    for (int i = 0; i < n; ++i) x(i, 0) = b(lu_result.piv[i], 0);

    // Forward substitution: solve Ly = Pb
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            x(i, 0) -= lu(i, j) * x(j, 0);
        }
        // L has 1s on diagonal
    }

    // Back substitution: solve Ux = y
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            x(i, 0) -= lu(i, j) * x(j, 0);
        }
        float diag = lu(i, i);
        if (std::fabs(static_cast<double>(diag)) < 1e-12) throw std::runtime_error("lu_solve: singular matrix");
        x(i, 0) /= diag;
    }

    return x;
}

} // namespace matrixlib
