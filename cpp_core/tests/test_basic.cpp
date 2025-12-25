#include "matrix/matrix.hpp"
#include <gtest/gtest.h>

using namespace matrixlib;

TEST(Addition, Simple) {
    Matrix a(2,2); a(0,0)=1; a(0,1)=2; a(1,0)=3; a(1,1)=4;
    Matrix b(2,2); b(0,0)=5; b(0,1)=6; b(1,0)=7; b(1,1)=8;
    Matrix c = add(a,b);
    EXPECT_FLOAT_EQ(c(0,0), 6);
    EXPECT_FLOAT_EQ(c(1,1), 12);
}

TEST(Addition, ZerosNegatives) {
    Matrix a(2,2); a(0,0)=0; a(0,1)=-2; a(1,0)=3; a(1,1)=0;
    Matrix b(2,2); b(0,0)=0; b(0,1)=2; b(1,0)=-3; b(1,1)=0;
    Matrix c = add(a,b);
    EXPECT_FLOAT_EQ(c(0,0), 0);
    EXPECT_FLOAT_EQ(c(0,1), 0);
    EXPECT_FLOAT_EQ(c(1,0), 0);
    EXPECT_FLOAT_EQ(c(1,1), 0);
}

TEST(Addition, SizeMismatchThrows) {
    Matrix a(2,2), b(2,3);
    EXPECT_THROW(add(a,b), std::invalid_argument);
}
