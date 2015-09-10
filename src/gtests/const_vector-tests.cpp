//
// Created by marandil on 08.09.15.
//

#include <gtest/gtest.h>

#include <mdlutils/types/const_vector.hpp>

int moves = 0;
int copies = 0;
int defaults = 0;
int parametrised = 0;
int constructs = 0;
int destroys = 0;

class ConstVectorTests : public ::testing::Test
{
public:
    ConstVectorTests()
    {
        moves = 0;
        copies = 0;
        defaults = 0;
        parametrised = 0;
        constructs = 0;
        destroys = 0;
    }
protected:
    struct noncopy
    {
        int v;

        noncopy() : v(0) { }

        noncopy(int i) : v(i) { }

        noncopy(const noncopy &) = delete;

        noncopy(noncopy &&) = delete;
    };

    struct noncopynodefault
    {
        int v;

        noncopynodefault(int i) : v(i) { }

        noncopynodefault(const noncopynodefault &) = delete;

        noncopynodefault(noncopynodefault &&) = delete;
    };

    struct refcounter
    {
        refcounter()
        {
            constructs++;
            defaults++;
        }

        refcounter(int)
        {
            constructs++;
            parametrised++;
        }

        refcounter(const refcounter &)
        {
            constructs++;
            copies++;
        }

        refcounter(refcounter &&)
        {
            constructs++;
            moves++;
        }

        ~refcounter() { destroys++; }
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

/* This should not compile
TEST_F(ConstVectorTests, NonCopyNoDefaultDefault)
{
    mdl::const_vector<noncopynodefault> vector(10);
    for (int i = 0; i < 10; ++i)
            EXPECT_EQ(0, vector[i].v);
}
//*/

TEST_F(ConstVectorTests, NonCopyNoDefaultCreate)
{
    mdl::const_vector<noncopynodefault> vector(10, 1);
    for (int i = 0; i < 10; ++i)
            EXPECT_EQ(1, vector[i].v);
}

TEST_F(ConstVectorTests, NonCopyNoDefaultMakeIndexed)
{
    mdl::const_vector<noncopynodefault> vector = mdl::const_vector<noncopynodefault>::make_indexed(10);
    for (int i = 0; i < 10; ++i)
            EXPECT_EQ(i, vector[i].v);
}

TEST_F(ConstVectorTests, RefCounterCreatesDefault)
{
    {
        mdl::const_vector<refcounter> vector(10);
        EXPECT_EQ(10, constructs);
        EXPECT_EQ(10, defaults);
        EXPECT_EQ(0, parametrised);
        EXPECT_EQ(0, copies);
        EXPECT_EQ(0, moves);
    }
    EXPECT_EQ(constructs, destroys);
}

TEST_F(ConstVectorTests, RefCounterCreatesCopy)
{
    {
        refcounter r;
        EXPECT_EQ(1, constructs);
        EXPECT_EQ(1, defaults);
        EXPECT_EQ(0, parametrised);
        EXPECT_EQ(0, copies);
        EXPECT_EQ(0, moves);

        mdl::const_vector<refcounter> vector(10, r);
        EXPECT_EQ(11, constructs);
        EXPECT_EQ(10, copies);
        EXPECT_EQ(1, defaults);
        EXPECT_EQ(0, parametrised);
        EXPECT_EQ(0, moves);
    }
    EXPECT_EQ(constructs, destroys);
}

TEST_F(ConstVectorTests, RefCounterCreatesCopy2)
{
    {
        mdl::const_vector<refcounter> vector(10, refcounter(1));
        EXPECT_EQ(11, constructs);
        EXPECT_EQ(10, copies);
        EXPECT_EQ(0, defaults);
        EXPECT_EQ(1, parametrised);
        EXPECT_EQ(0, moves);
    }
    EXPECT_EQ(constructs, destroys);
}

TEST_F(ConstVectorTests, RefCounterMakeIndexed)
{
    {
        mdl::const_vector<refcounter> vector = mdl::const_vector<refcounter>::make_indexed(10);
        EXPECT_EQ(10, constructs);
        EXPECT_EQ(0, copies);
        EXPECT_EQ(0, defaults);
        EXPECT_EQ(10, parametrised);
        EXPECT_EQ(0, moves);
    }
    EXPECT_EQ(constructs, destroys);
}

TEST_F(ConstVectorTests, RefCounterMoveMove)
{
    {
        mdl::const_vector<refcounter> vector = mdl::const_vector<refcounter>::make_indexed(10);
        mdl::const_vector<refcounter> moved = std::move(vector);
        EXPECT_EQ(10, constructs);
        EXPECT_EQ(0, copies);
        EXPECT_EQ(0, defaults);
        EXPECT_EQ(10, parametrised);
        EXPECT_EQ(0, moves);
    }
    EXPECT_EQ(constructs, destroys);
}