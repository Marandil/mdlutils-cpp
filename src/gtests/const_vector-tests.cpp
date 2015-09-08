//
// Created by marandil on 08.09.15.
//

#include <gtest/gtest.h>

#include <mdlutils/types/const_vector.hpp>

class ConstVectorTests : public ::testing::Test
{
protected:
    struct noncopy
    {
        int v;

        noncopy() : v(0) { }

        noncopy(int i) : v(i) { }

        noncopy(const noncopy &) = delete;

        noncopy(noncopy &&) = delete;
    };
};

TEST_F(ConstVectorTests, NonCopyDefault)
{
    mdl::const_vector<noncopy> vector(10);
    for (int i = 0; i < 10; ++i)
            EXPECT_EQ(0, vector[i].v);
}

TEST_F(ConstVectorTests, NonCopyCreate)
{
    mdl::const_vector<noncopy> vector(10, 1);
    for (int i = 0; i < 10; ++i)
            EXPECT_EQ(1, vector[i].v);
}

TEST_F(ConstVectorTests, NonCopyMakeIndexed)
{
    mdl::const_vector<noncopy> vector = mdl::const_vector<noncopy>::make_indexed(10);
    for (int i = 0; i < 10; ++i)
            EXPECT_EQ(i, vector[i].v);
}