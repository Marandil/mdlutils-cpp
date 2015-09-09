//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_HANDLER_HPP
#define MDLUTILS_MULTITHREADING_HANDLER_HPP

#include <mdlutils/multithreading/messages.hpp>

namespace mdl
{
    class handler
    {
    public:
        virtual void handle_message(message_ptr) = 0;
    };
}

#endif //MDLUTILS_MULTITHREADING_HANDLER_HPP
