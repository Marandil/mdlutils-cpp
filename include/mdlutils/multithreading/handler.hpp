//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_HANDLER_HPP
#define MDLUTILS_MULTITHREADING_HANDLER_HPP

#include <mdlutils/exceptions/exception_handler.hpp>
#include <mdlutils/multithreading/messages.hpp>

namespace mdl
{
    class handler
    {
    public:
        virtual void handle_message(message_ptr) = 0;
    };


    class executor_handler : public handler
    {
    protected:
        exception_handler& exception_h;
    public:
        executor_handler(exception_handler& exception_handler) : exception_h(exception_handler) { }
        virtual void handle_message(message_ptr);
    };
}

#endif //MDLUTILS_MULTITHREADING_HANDLER_HPP
