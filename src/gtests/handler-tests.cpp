//
// Created by marandil on 11.09.15.
//

#include <list>
#include <queue>

#include <gtest/gtest.h>

#include <mdlutils/multithreading/handler.hpp>


class HandlerTest : public ::testing::Test
{
protected:
    std::queue<int> history;

    class handlerA : public mdl::handler
    {
    public:
        HandlerTest *parent;

        handlerA(HandlerTest *parent) : parent(parent) { }

        virtual bool handle_message(mdl::message_ptr msg)
        {
            parent->history.push(1);
            return true;
        }
    };

    class handlerB : public mdl::handler
    {
    public:
        HandlerTest *parent;

        handlerB(HandlerTest *parent) : parent(parent) { }

        virtual bool handle_message(mdl::message_ptr msg)
        {
            parent->history.push(2);
            return true;
        }
    };

    class A : public handlerA
    {
    protected:
        std::list<mdl::handler *> lst;
    public:
        A(HandlerTest *parent) : handlerA(parent) { lst.push_back(&static_cast<handlerA &>(*this)); }

        virtual void run()
        {
            for (auto item : lst)
            {
                item->handle_message(nullptr);
            }
        }
    };

    class B : public A, public handlerB
    {
    public:
        B(HandlerTest *parent) : A(parent), handlerB(parent) { lst.push_back(&static_cast<handlerB &>(*this)); }
    };
};

TEST_F(HandlerTest, HandleMessage)
{
    B b(this);
    b.run();
    EXPECT_EQ(1, history.front());
    history.pop();
    EXPECT_EQ(2, history.front());
    history.pop();
    EXPECT_TRUE(history.empty());
}