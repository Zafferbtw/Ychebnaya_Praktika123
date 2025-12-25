#include "matrix/matrix.hpp"
#include <gtest/gtest.h>
#include <cmath>

using namespace matrixlib;

TEST(Determinant, Identity) {
    Matrix I = Matrix::identity(4);
    EXPECT_NEAR(det_small(I), 1.0, 1e-6);
}

TEST(Determinant, Known3x3) {
    // det of:
    // [1 2 3]
    // [0 1 4]
    // [5 6 0] = 1*(1*0-4*6) -2*(0*0-4*5) +3*(0*6-1*5) = -24 +40 -15 = 1
    Matrix A(3,3);
    A(0,0)=1; A(0,1)=2; A(0,2)=3;
    A(1,0)=0; A(1,1)=1; A(1,2)=4;
    A(2,0)=5; A(2,1)=6; A(2,2)=0;
    EXPECT_NEAR(det_small(A), 1.0, 1e-6);
}

TEST(Determinant, TooLargeThrows) {
    Matrix A(11,11,0.0f);
    EXPECT_THROW(det_small(A), std::invalid_argument);
}
