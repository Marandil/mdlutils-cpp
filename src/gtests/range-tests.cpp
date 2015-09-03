//
// Created by marandil on 03.09.15.
//

//
// Created by marandil on 03.09.15.
//

#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include <mdlutils/types/range.hpp>

class RangeTest : public ::testing::Test
{
protected:
    template<typename T>
    struct args
    {
        T s, e, o;
    };

    std::vector<std::pair<args<int>, std::vector<int>>> pythonRRT;

public:
    RangeTest()
    {
        // Python random range emulation tests:
        // ( check gen_range_tests.py for details on generation )
#include "pythonrtt.inc"
    }
};

template<typename T>
void test_range(T first, T last, T offset, mdl::range<T> r)
{
    size_t n(0);

    // count the actual number of occurrences in the classic range case
    if (offset > 0)
        for (T i = first; i < last; i += offset)
            n++;
    else
        for (T i = first; i > last; i += offset)
            n++;

    for (T i : r)
    {
        n--;
        EXPECT_EQ(first, i);
        first += offset;
    }
    EXPECT_EQ(0, n);
}

template<typename T>
void test_simple_range(T last)
{
    mdl::range<T> r(last);
    T first(0), offset(1);

    test_range(first, last, offset, r);
}

template<typename T>
void test_se_range(T first, T last)
{
    mdl::range<T> r(first, last);
    T offset(1);

    test_range(first, last, offset, r);
}

template<typename T>
void test_seo_range(T first, T last, T offset)
{
    mdl::range<T> r(first, last, offset);

    test_range(first, last, offset, r);
}

TEST_F(RangeTest, IntSimple)
{
    test_simple_range(0);
    test_simple_range(1);
    test_simple_range(2);
    test_simple_range(10);
}

TEST_F(RangeTest, IntStartStop)
{
    test_se_range(0, 0);
    test_se_range(0, 1);
    test_se_range(0, 2);
    test_se_range(0, 10);

    test_se_range(-7, 0);
    test_se_range(12, 1);
    test_se_range(2, 2);
    test_se_range(-10, 10);
}

TEST_F(RangeTest, IntStartStopOff)
{
    test_seo_range(0, 0, 1);
    test_seo_range(0, 1, 1);
    test_seo_range(0, 2, 1);
    test_seo_range(0, 10, 1);

    test_seo_range(-7, 0, 1);
    test_seo_range(12, 1, -1);
    test_seo_range(2, 2, 10);
    test_seo_range(-10, 10, 5);
}

TEST_F(RangeTest, PythonRRT)
{
    for (auto &item : pythonRRT)
    {
        mdl::range<int> r(item.first.s, item.first.e, item.first.o);
        std::vector<int> test(r.begin(), r.end());
        EXPECT_EQ(test, item.second);
    }
}
