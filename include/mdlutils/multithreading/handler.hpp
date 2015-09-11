//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_HANDLER_HPP
#define MDLUTILS_MULTITHREADING_HANDLER_HPP

#include <queue>

#include <mdlutils/exceptions/exception_handler.hpp>
#include <mdlutils/multithreading/messages.hpp>

namespace mdl
{
    class handler
    {
    public:
        virtual bool handle_message(message_ptr) = 0;
    };

    class delaying_handler : public handler
    {
    protected:
        std::queue<message_ptr>& queue;
        std::mutex& queue_lock;
    public:
        delaying_handler(std::queue<message_ptr>& queue, std::mutex& lock) : queue(queue), queue_lock(lock) { }
        virtual bool handle_message(message_ptr);
    };

    class executor_handler : public handler
    {
    public:
        virtual bool handle_message(message_ptr);
    };

    class break_handler : public handler
    {
    public:
        virtual bool handle_message(message_ptr);
    };
}

#endif //MDLUTILS_MULTITHREADING_HANDLER_HPP
