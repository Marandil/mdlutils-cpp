//
// Created by marandil on 11.09.15.
//

#include <gtest/gtest.h>

#include <mdlutils/multithreading/looper.hpp>

std::deque<int> history;

class LooperTest : public ::testing::Test
{
protected:
    LooperTest() { history.clear(); }

    struct null_handler : mdl::handler
    {
        virtual bool handle_message(mdl::message_ptr msg)
        {
            if (msg == nullptr)
            {
                history.push_back(0);
                return true;
            }
            return false;
        }
    };

    null_handler n_handler;

    mdl::looper_thread looper = {n_handler};
};

TEST_F(LooperTest, CreateAndExit)
{
    EXPECT_TRUE(true);
}