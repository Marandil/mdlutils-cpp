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
    /* Base class for message handlers.
     *
     * All message handlers should public-extend this class */
    class handler
    {
    public:
        /* Abstract method for handling the message.
         * @message_ptr shared pointer to a message that's about to be handled.
         *
         * @return true if the message has been completely processed by the handler, false otherwise.
         */
        virtual bool handle_message(message_ptr) = 0;
    };

    /* Message handler automatically processing delayed_message messages.
     *
     * Requires access to message queue and it's mutex
     */
    class delaying_handler : public handler
    {
    protected:
        std::queue<message_ptr>& queue;
        std::mutex& queue_lock;
    public:
        /* Construct delaying_handler for the given message queue and mutex
         * @queue message queue, where delayed messages will be placed
         * @lock std::mutex of the message queue.
         */
        delaying_handler(std::queue<message_ptr>& queue, std::mutex& lock) : queue(queue), queue_lock(lock) { }
        // @inherit
        virtual bool handle_message(message_ptr);
    };

    /* Message handler automatically processing post_call messages.
     *
     * Runs functions passed along with the post_call messages.
     */
    class executor_handler : public handler
    {
    public:
        // @inherit
        virtual bool handle_message(message_ptr);
    };

    /* Message handler automatically processing break_message messages.
     *
     * Breaks out of message loops by throwing break_out_exception.
     */
    class break_handler : public handler
    {
    public:
        // @inherit
        virtual bool handle_message(message_ptr);
    };
}

#endif //MDLUTILS_MULTITHREADING_HANDLER_HPP
