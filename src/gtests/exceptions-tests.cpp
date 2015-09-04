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

void function()
{
    std::string function = "void function()";
    try
    {
        mdl_throw(mdl::base_exception, "test");
    }
    catch (mdl::base_exception &e)
    {
        EXPECT_EQ(function, e.throw_function());
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

TEST_F(ExceptionsTest, BaseExceptionFunction)
{
    function();
}

TEST_F(ExceptionsTest, BaseExceptionWhat)
{
    int line = 0;
    std::string filename, function;
    try
    {
        filename = __FILE__;
        function = __NICE_FUNCTION__;
        line = __LINE__ + 1;
        mdl_throw(mdl::base_exception, "test");
    }
    catch (mdl::base_exception &e)
    {
        std::string expected(
                "BaseException: test\n\tin function " + function + "\n\tat " + std::to_string(line) + " in " +
                filename);
        EXPECT_EQ(expected, e.what()); // should invoke std::string(const char*) constructor
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

TEST_F(ExceptionsTest, NotImplementedExceptionWhat)
{
    int line = 0;
    std::string filename, function;
    try
    {
        filename = __FILE__;
        function = __NICE_FUNCTION__;
        line = __LINE__ + 1;
        mdl_throw(mdl::not_implemented_exception, "test");
    }
    catch (mdl::base_exception &e)
    {
        std::string expected(
                "NotImplementedException: test\n\tin function " + function + "\n\tat " + std::to_string(line) + " in " +
                filename);
        EXPECT_EQ(expected, e.what()); // should invoke std::string(const char*) constructor
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

template<typename T>
void test_numeric_invalid_argument_exception(T number)
{
    try
    {
        mdl_throw(mdl::invalid_argument_exception<T>, "number", number);
    }
    catch (mdl::base_exception &e)
    {
        std::string expected("InvalidArgumentException: Argument number with value " + std::to_string(number));
        std::string what = e.what();
        // shrink what so that it does not contain the parametric values (like function, line, etc)
        what = what.substr(0, what.find("\n\tin"));
        EXPECT_EQ(expected, what); // should invoke std::string(const char*) constructor
        return;
    }
    EXPECT_TRUE(false); // Didn't catch
}

TEST_F(ExceptionsTest, IntegerInvalidArgumentException)
{
    test_numeric_invalid_argument_exception<int8_t>(1);
    test_numeric_invalid_argument_exception<int16_t>(1);
    test_numeric_invalid_argument_exception<int32_t>(1);
    test_numeric_invalid_argument_exception<int64_t>(1);

    test_numeric_invalid_argument_exception<uint8_t>(1);
    test_numeric_invalid_argument_exception<uint16_t>(1);
    test_numeric_invalid_argument_exception<uint32_t>(1);
    test_numeric_invalid_argument_exception<uint64_t>(1);
}

TEST_F(ExceptionsTest, FloatInvalidArgumentException)
{
    test_numeric_invalid_argument_exception<float>(1);
    test_numeric_invalid_argument_exception<double>(1);
    test_numeric_invalid_argument_exception<long double>(1);
}
