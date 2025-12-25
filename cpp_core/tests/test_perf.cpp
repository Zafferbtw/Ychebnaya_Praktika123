#include "matrix/matrix.hpp"
#include <gtest/gtest.h>
#include <chrono>

using namespace matrixlib;

TEST(Performance, Mul100x100Stress) {
    const int n = 100;
    Matrix A(n,n,1.0f);
    Matrix B(n,n,1.0f);

    // single run + basic time measurement (not a strict benchmark)
    auto t0 = std::chrono::high_resolution_clock::now();
    Matrix C = multiply(A,B);
    auto t1 = std::chrono::high_resolution_clock::now();

    // C should be all n
    EXPECT_FLOAT_EQ(C(0,0), static_cast<float>(n));
    EXPECT_FLOAT_EQ(C(n-1,n-1), static_cast<float>(n));

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    // do not assert time; just print for report
    std::cout << "mul 100x100 time: " << ms << " ms\n";
}
