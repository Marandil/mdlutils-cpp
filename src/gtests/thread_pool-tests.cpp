//
// Created by marandil on 14.09.15.
//

#include <mdlutils/multithreading/thread_pool.hpp>

#include <gtest/gtest.h>
#include <mdlutils/types/range.hpp>

class ThreadPoolTest : public ::testing::Test
{
protected:

};

TEST_F(ThreadPoolTest, CreateAndExit)
{
    mdl::thread_pool pool;
    EXPECT_TRUE(true);
}

int add(int a, int b)
{
    return a + b;
}

void simple_add_test(mdl::thread_pool &pool)
{
    int a = 10, b = 15;
    std::future<int> c = pool.async(add, a, b);
    EXPECT_EQ(a + b, c.get());
}

template<size_t n>
void multiple_add_test(mdl::thread_pool &pool)
{
    std::array<int, n> a, b;
    std::array<std::future<int>, n> c;
    for (size_t i : mdl::range<size_t>(n))
    {
        a[i] = i;
        b[i] = 5 * i;
    }
    for (size_t i : mdl::range<size_t>(n))
        c[i] = pool.async(add, a[i], b[i]);
    for (size_t i : mdl::range<size_t>(n))
            EXPECT_EQ(a[i] + b[i], c[i].get());
}

TEST_F(ThreadPoolTest, SimpleAddRoundRobin)
{
    mdl::thread_pool pool(1, mdl::thread_pool::strategy::round_robin);
    simple_add_test(pool);
}

TEST_F(ThreadPoolTest, MultipleAddRoundRobin)
{
    mdl::thread_pool pool(4, mdl::thread_pool::strategy::round_robin);
    multiple_add_test<100>(pool);
}

TEST_F(ThreadPoolTest, SimpleAddDynamic)
{
    mdl::thread_pool pool(1, mdl::thread_pool::strategy::dynamic);
    simple_add_test(pool);
}

TEST_F(ThreadPoolTest, MultipleAddDynamic)
{
    mdl::thread_pool pool(4, mdl::thread_pool::strategy::dynamic);
    multiple_add_test<100>(pool);
}

TEST_F(ThreadPoolTest, MapRoundRobin)
{
    mdl::thread_pool pool(4, mdl::thread_pool::strategy::dynamic);
    std::array<int, 10> a, b;
    pool.map(a.end(), a.begin(), b.begin());
}
