#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <cstddef>
#include <initializer_list>

/**
 * @file matrix.hpp
 * @brief Dense matrix library for practice tasks.
 */

namespace matrixlib {

/**
 * @brief Dense matrix of floats stored in row-major contiguous memory.
 *
 * Intended size: up to 100x100.
 */
class Matrix {
public:
    using value_type = float;

    Matrix() = default;
    Matrix(std::size_t rows, std::size_t cols, value_type init = 0.0f);

    std::size_t rows() const noexcept { return r_; }
    std::size_t cols() const noexcept { return c_; }
    bool empty() const noexcept { return data_.empty(); }

    value_type* data() noexcept { return data_.data(); }
    const value_type* data() const noexcept { return data_.data(); }

    value_type& operator()(std::size_t i, std::size_t j);
    value_type  operator()(std::size_t i, std::size_t j) const;

    const std::vector<value_type>& vec() const noexcept { return data_; }
    std::vector<value_type>& vec() noexcept { return data_; }

    static Matrix identity(std::size_t n);

private:
    std::size_t r_{0}, c_{0};
    std::vector<value_type> data_;
};

/// Basic operations
Matrix add(const Matrix& a, const Matrix& b);
Matrix sub(const Matrix& a, const Matrix& b);
Matrix scalar_mul(const Matrix& a, Matrix::value_type k);

/// Transpose
Matrix transpose(const Matrix& a);

/// Trace (requires square)
double trace(const Matrix& a);

/// Determinant for small sizes (n <= 10), requires square
double det_small(const Matrix& a);

/// Matrix multiplication (square A(nxn) * B(nxn))
Matrix multiply(const Matrix& a, const Matrix& b);

/// Explicit implementations for task 1
void matrix_multiply_high_level(const float* A, const float* B, float* C, int n);
void matrix_multiply_low_level(const float* A, const float* B, float* C, int n);

/// LU decomposition and solve (Ax=b)
struct LUResult {
    Matrix lu;                 ///< combined L and U in one matrix
    std::vector<int> piv;      ///< pivot indices (row swaps)
    int piv_sign{1};           ///< +1 or -1 depending on number of swaps
};

/**
 * @brief LU decomposition with partial pivoting.
 * @throws std::invalid_argument on non-square matrix
 * @throws std::runtime_error on singular matrix
 */
LUResult lu_decompose(const Matrix& a);

/**
 * @brief Solve Ax=b using LU decomposition result.
 * @param lu_result output of lu_decompose(a)
 * @param b right-hand side vector as column matrix (n x 1)
 * @return x as column matrix (n x 1)
 */
Matrix lu_solve(const LUResult& lu_result, const Matrix& b);

/// Simple I/O helpers for console app
Matrix read_matrix_from_stream(std::istream& in);
void write_matrix_to_stream(std::ostream& out, const Matrix& m, int precision = 6);

} // namespace matrixlib
