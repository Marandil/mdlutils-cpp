//
// Created by marandil on 04.09.15.
//

#include <gtest/gtest.h>
#include <mdlutils/accessor/simple_accessor.hpp>

class SimpleAccessorTest : public ::testing::Test
{
protected:
    int a, b, c;
public:
    mdl::simple_accessor<int> A{a}, B{b}, C{c};

    void test_add(int, int);

    void test_sub(int, int);

    void test_mul(int, int);

    void test_div(int, int);
};

TEST_F(SimpleAccessorTest, FieldAssignment)
{
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ(1, A);
    EXPECT_EQ(2, B);
    EXPECT_EQ(3, C);
    EXPECT_EQ(a, A);
    EXPECT_EQ(b, B);
    EXPECT_EQ(c, C);
}

TEST_F(SimpleAccessorTest, AccessorAssignment)
{
    A = 1;
    B = 2;
    C = 3;
    EXPECT_EQ(1, a);
    EXPECT_EQ(2, b);
    EXPECT_EQ(3, c);
    EXPECT_EQ(a, A);
    EXPECT_EQ(b, B);
    EXPECT_EQ(c, C);
}

void SimpleAccessorTest::test_add(int op1, int op2)
{
    a = op1;
    A = A + op2;
    EXPECT_EQ(A, op1 + op2);
    b = op1;
    B += op2;
    EXPECT_EQ(B, op1 + op2);
    EXPECT_EQ(A, B);
}

void SimpleAccessorTest::test_sub(int op1, int op2)
{
    a = op1;
    A = A - op2;
    EXPECT_EQ(A, op1 - op2);
    b = op1;
    B -= op2;
    EXPECT_EQ(B, op1 - op2);
    EXPECT_EQ(A, B);
}

void SimpleAccessorTest::test_mul(int op1, int op2)
{
    a = op1;
    A = A * op2;
    EXPECT_EQ(A, op1 * op2);
    b = op1;
    B *= op2;
    EXPECT_EQ(B, op1 * op2);
    EXPECT_EQ(A, B);
}

void SimpleAccessorTest::test_div(int op1, int op2)
{
    a = op1;
    A = A / op2;
    EXPECT_EQ(A, op1 / op2);
    b = op1;
    B /= op2;
    EXPECT_EQ(B, op1 / op2);
    EXPECT_EQ(A, B);
}


TEST_F(SimpleAccessorTest, AccessorAddition)
{
    test_add(5, 10);
    test_add(-5, 5);
    test_add(200, 500);
}


TEST_F(SimpleAccessorTest, AccessorSubtraction)
{
    test_sub(5, 10);
    test_sub(-5, 5);
    test_sub(200, 500);
}


TEST_F(SimpleAccessorTest, AccessorMultiplication)
{
    test_mul(5, 10);
    test_mul(-5, 5);
    test_mul(200, 500);
}


TEST_F(SimpleAccessorTest, AccessorDivision)
{
    test_div(100, 2);
    test_div(-5, 5);
    test_div(0x5df331, 0x15);
}

TEST_F(SimpleAccessorTest, AccessorRelations)
{
    a = 1;
    b = 2;
    c = 3;

    EXPECT_TRUE(A == 1);
    EXPECT_TRUE(A != 2);
    EXPECT_FALSE(A != 1);
    EXPECT_FALSE(A == 2);

    EXPECT_TRUE(A < B);
    EXPECT_TRUE(A < b);
    EXPECT_TRUE(A < 2);

    EXPECT_FALSE(B < A);
    EXPECT_FALSE(B < a);
    EXPECT_FALSE(B < 1);

    EXPECT_FALSE(A > B);
    EXPECT_FALSE(A > b);
    EXPECT_FALSE(A > 2);

    EXPECT_TRUE(B > A);
    EXPECT_TRUE(B > a);
    EXPECT_TRUE(B > 1);

    EXPECT_TRUE(A <= B);
    EXPECT_TRUE(A <= b);
    EXPECT_TRUE(A <= 2);

    EXPECT_FALSE(B <= A);
    EXPECT_FALSE(B <= a);
    EXPECT_FALSE(B <= 1);

    EXPECT_FALSE(A >= B);
    EXPECT_FALSE(A >= b);
    EXPECT_FALSE(A >= 2);

    EXPECT_TRUE(B >= A);
    EXPECT_TRUE(B >= a);
    EXPECT_TRUE(B >= 1);

    EXPECT_EQ(std::max<int>({A, B, C}), 3);
    EXPECT_EQ(std::min<int>({A, B, C}), 1);
}
