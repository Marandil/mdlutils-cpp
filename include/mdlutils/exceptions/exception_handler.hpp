//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_EXCEPTIONS_EXCEPTION_HANDLER_HPP
#define MDLUTILS_EXCEPTIONS_EXCEPTION_HANDLER_HPP

#include <exception>

namespace mdl
{
    class exception_handler
    {
    public:
        virtual void handle_exception(std::exception_ptr) = 0;
    };

    class naive_rethrow : public exception_handler
    {
    public:
        virtual void handle_exception(std::exception_ptr ptr)
        {
            std::rethrow_exception(ptr);
        }
    };
}

#endif //MDLUTILS_EXCEPTIONS_EXCEPTION_HANDLER_HPP
