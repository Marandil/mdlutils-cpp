//
// Created by marandil on 01.09.15.
//

#include <gtest/gtest.h>
#include <mdlutils/exceptions.hpp>

// The fixture for testing class Foo.
class ExceptionsTest : public ::testing::Test
{
protected:
    ExceptionsTest()
    {
        // You can do set-up work for each test here.
    }

    virtual ~ExceptionsTest()
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp()
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown()
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
};


TEST_F(ExceptionsTest, BaseExceptionLineNo)
{
    int line = 0;
    try
    {
        line = __LINE__ + 1;
        mdl_throw(mdl::base_exception, "test");
    }
    catch (mdl::base_exception &e)
    {
        EXPECT_EQ(line, e.throw_line());
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

TEST_F(ExceptionsTest, BaseExceptionFilename)
{
    std::string filename;
    try
    {
        filename = __FILE__;
        mdl_throw(mdl::base_exception, "test");
    }
    catch (mdl::base_exception &e)
    {
        EXPECT_EQ(filename, e.throw_file());
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

TEST_F(ExceptionsTest, BaseExceptionWhat)
{
    int line = 0;
    std::string filename;
    try
    {
        filename = __FILE__;
        line = __LINE__ + 1;
        mdl_throw(mdl::base_exception, "test");
    }
    catch (mdl::base_exception &e)
    {
        std::string expected("BaseException: test\n\tat " + std::to_string(line) + " in " + filename);
        EXPECT_EQ(expected, e.what()); // should invoke std::string(const char*) constructor
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

TEST_F(ExceptionsTest, NotImplementedExceptionWhat)
{
    int line = 0;
    std::string filename;
    try
    {
        filename = __FILE__;
        line = __LINE__ + 1;
        mdl_throw(mdl::not_implemented_exception, "test");
    }
    catch (mdl::base_exception &e)
    {
        std::string expected("NotImplementedException: test\n\tat " + std::to_string(line) + " in " + filename);
        EXPECT_EQ(expected, e.what()); // should invoke std::string(const char*) constructor
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
