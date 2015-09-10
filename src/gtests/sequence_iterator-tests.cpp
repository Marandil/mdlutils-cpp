//
// Created by marandil on 03.09.15.
//

#include <utility>

#include <gtest/gtest.h>

#include <mdlutils/types/sequence_iterator.hpp>

class SequenceIteratorTest : public ::testing::Test
{
protected:


public:
    SequenceIteratorTest()
    {

    }
};

template<typename T>
void test_dereference_operator(T a)
{
    mdl::sequence_iterator<T> it(a);
    EXPECT_EQ(*it, a);
}

template<typename T>
void test_swap_operator(T a, T b)
{
    mdl::sequence_iterator<T> ia(a), ib(b);
    EXPECT_EQ(*ia, a);
    EXPECT_EQ(*ib, b);
    std::swap(ia, ib);
    EXPECT_EQ(*ia, b);
    EXPECT_EQ(*ib, a);
}

template<typename T>
void test_basic_range_inc(T a, T b, T o)
{
    mdl::sequence_iterator<T> ia(a, o), ib(b, o);

    ptrdiff_t n = (b - a);
    T x(a);
    for (; ia != ib; ++ia)
    {
        EXPECT_EQ(*ia, x);
        x += o;
        n -= o;
    }
    EXPECT_EQ(0, n);
}

template<typename T>
void test_basic_range_dec(T a, T b, T o)
{
    mdl::sequence_iterator<T> ia(a, o), ib(b, o);

    ptrdiff_t n = (b - a);
    T x(b);
    for (; ia != ib; --ib)
    {
        EXPECT_EQ(*ib, x);
        x -= o;
        n -= o;
    }
    EXPECT_EQ(0, n);
}

TEST_F(SequenceIteratorTest, IntDereferenceOperator)
{
    test_dereference_operator(1);
    test_dereference_operator(2);
    test_dereference_operator(10);
}

TEST_F(SequenceIteratorTest, IntSwapOperator)
{
    test_swap_operator(1, 5);
    test_swap_operator(10, -10);
}

TEST_F(SequenceIteratorTest, IntBasicRangeInc)
{
    test_basic_range_inc(1, 5, 1);
    test_basic_range_inc(0, 1000, 2);
    test_basic_range_inc(5, 1, -1);
    test_basic_range_inc(1000, 0, -2);
}

TEST_F(SequenceIteratorTest, IntBasicRangeDec)
{
    test_basic_range_dec(1, 5, 1);
    test_basic_range_dec(0, 1000, 2);
    test_basic_range_dec(5, 1, -1);
    test_basic_range_dec(1000, 0, -2);
}
