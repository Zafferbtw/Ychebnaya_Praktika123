#include "matrix/matrix.hpp"
#include <gtest/gtest.h>
#include <cmath>

using namespace matrixlib;

TEST(LU, SolveSimple) {
    // A = [[3,1],[1,2]], b = [9,8]
    // solution x = [2,3]
    Matrix A(2,2);
    A(0,0)=3; A(0,1)=1;
    A(1,0)=1; A(1,1)=2;

    Matrix b(2,1);
    b(0,0)=9;
    b(1,0)=8;

    LUResult lu = lu_decompose(A);
    Matrix x = lu_solve(lu, b);

    EXPECT_NEAR(x(0,0), 2.0, 1e-5);
    EXPECT_NEAR(x(1,0), 3.0, 1e-5);
}
