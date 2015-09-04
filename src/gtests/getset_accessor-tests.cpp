//
// Created by marandil on 04.09.15.
//


#include <gtest/gtest.h>
#include <mdlutils/accessor/getset_accessor.hpp>

#include <vector>

class GetSetAccessorTest : public ::testing::Test
{
protected:
    int a{0};
    size_t s = 1024;

    std::vector<int> vec;
public:
    mdl::get_accessor<int> gA = {[&]()
                                     {
                                         return a;
                                     }};

    mdl::set_accessor<int> sA = {[&](int value)
                                     {
                                         a = value;
                                     }};

    mdl::getset_accessor<int> A = {
            [&]()
                {
                    return a;
                },
            [&](int value)
                {
                    a = value;
                }
    };

    mdl::get_accessor<size_t> size = {[&]
                                          {
                                              return vec.size();
                                          }};

    GetSetAccessorTest()
    {
        vec.resize(s);
    }
};

TEST_F(GetSetAccessorTest, LambdaUsage)
{
    std::function<int(void)> simple_getter;
    for (int i = 0; i < 10; ++i)
        simple_getter = [&]()
            { return i; };
    EXPECT_EQ(simple_getter(), 10);
}

TEST_F(GetSetAccessorTest, GetAccessorAccess)
{
    int val = 10;
    a = val;
    int test = gA;
    EXPECT_EQ(test, val);
}

TEST_F(GetSetAccessorTest, SetAccessorSetting)
{
    int val = 10;
    sA = val;
    EXPECT_EQ(a, val);
}

TEST_F(GetSetAccessorTest, GetSetAccessorAccess)
{
    int val = 10;
    a = val;
    int test = A;
    EXPECT_EQ(test, val);
}

TEST_F(GetSetAccessorTest, GetSetAccessorSetting)
{
    int val = 10;
    A = val;
    EXPECT_EQ(a, val);
}

TEST_F(GetSetAccessorTest, AllAccessorsCoop)
{
    {
        int val = 10;
        A = val;
        EXPECT_EQ(gA, val);
    }
    {
        int val = 20;
        sA = val;
        EXPECT_EQ(A, val);
    }
}

TEST_F(GetSetAccessorTest, ComplexGet)
{
    EXPECT_EQ(size, s);
}