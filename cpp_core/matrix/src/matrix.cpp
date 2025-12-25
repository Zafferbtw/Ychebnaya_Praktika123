#include "matrix/matrix.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace matrixlib {

Matrix::Matrix(std::size_t rows, std::size_t cols, value_type init)
    : r_(rows), c_(cols), data_(rows * cols, init) {
    if (rows == 0 || cols == 0) {
        // allow empty matrices, but keep consistent sizing
        if (rows != 0 || cols != 0) {
            throw std::invalid_argument("rows/cols must both be zero for empty matrix");
        }
    }
    if (rows > 100 || cols > 100) {
        throw std::invalid_argument("matrix size exceeds 100x100 limit");
    }
}

Matrix::value_type& Matrix::operator()(std::size_t i, std::size_t j) {
    if (i >= r_ || j >= c_) throw std::out_of_range("matrix index out of range");
    return data_[i * c_ + j];
}
Matrix::value_type Matrix::operator()(std::size_t i, std::size_t j) const {
    if (i >= r_ || j >= c_) throw std::out_of_range("matrix index out of range");
    return data_[i * c_ + j];
}

Matrix Matrix::identity(std::size_t n) {
    Matrix I(n, n, 0.0f);
    for (std::size_t i = 0; i < n; ++i) I(i, i) = 1.0f;
    return I;
}

static void ensure_same_size(const Matrix& a, const Matrix& b, const char* op) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
        throw std::invalid_argument(std::string(op) + ": size mismatch");
    }
}

Matrix add(const Matrix& a, const Matrix& b) {
    ensure_same_size(a, b, "add");
    Matrix out(a.rows(), a.cols());
    std::transform(a.vec().begin(), a.vec().end(), b.vec().begin(), out.vec().begin(),
                   [](float x, float y) { return x + y; });
    return out;
}

Matrix sub(const Matrix& a, const Matrix& b) {
    ensure_same_size(a, b, "sub");
    Matrix out(a.rows(), a.cols());
    std::transform(a.vec().begin(), a.vec().end(), b.vec().begin(), out.vec().begin(),
                   [](float x, float y) { return x - y; });
    return out;
}

Matrix scalar_mul(const Matrix& a, Matrix::value_type k) {
    Matrix out(a.rows(), a.cols());
    std::transform(a.vec().begin(), a.vec().end(), out.vec().begin(),
                   [k](float x) { return x * k; });
    return out;
}

Matrix transpose(const Matrix& a) {
    Matrix out(a.cols(), a.rows());
    for (std::size_t i = 0; i < a.rows(); ++i)
        for (std::size_t j = 0; j < a.cols(); ++j)
            out(j, i) = a(i, j);
    return out;
}

double trace(const Matrix& a) {
    if (a.rows() != a.cols()) throw std::invalid_argument("trace: matrix must be square");
    double s = 0.0;
    for (std::size_t i = 0; i < a.rows(); ++i) s += static_cast<double>(a(i, i));
    return s;
}

Matrix multiply(const Matrix& a, const Matrix& b) {
    if (a.rows() != a.cols() || b.rows() != b.cols()) {
        throw std::invalid_argument("multiply: only square matrices supported in this practice project");
    }
    if (a.rows() != b.rows()) throw std::invalid_argument("multiply: size mismatch");
    int n = static_cast<int>(a.rows());
    Matrix out(a.rows(), a.cols(), 0.0f);

    // If size divisible by 8, use AVX path; else fallback.
    if (n % 8 == 0) {
        matrix_multiply_low_level(a.data(), b.data(), out.data(), n);
    } else {
        matrix_multiply_high_level(a.data(), b.data(), out.data(), n);
    }
    return out;
}

Matrix read_matrix_from_stream(std::istream& in) {
    std::size_t r, c;
    if (!(in >> r >> c)) throw std::runtime_error("failed to read matrix header");
    Matrix m(r, c);
    for (std::size_t i = 0; i < r; ++i) {
        for (std::size_t j = 0; j < c; ++j) {
            float v;
            if (!(in >> v)) throw std::runtime_error("failed to read matrix element");
            m(i, j) = v;
        }
    }
    return m;
}

void write_matrix_to_stream(std::ostream& out, const Matrix& m, int precision) {
    out << m.rows() << " " << m.cols() << "\n";
    out << std::fixed << std::setprecision(precision);
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            out << m(i, j);
            if (j + 1 < m.cols()) out << " ";
        }
        out << "\n";
    }
}

} // namespace matrixlib
