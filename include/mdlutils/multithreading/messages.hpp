//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_MESSAGES_HPP
#define MDLUTILS_MULTITHREADING_MESSAGES_HPP

#include <memory>
#include <functional>

#include <mdlutils/multithreading/helpers.hpp>

namespace mdl
{
    struct message { virtual ~message() {} };

    struct post_call : public message
    {
        virtual ~post_call() {}
        std::function<void(void)> function;
    };

    struct delayed_message : public message
    {
        virtual ~delayed_message() {}
        std::shared_ptr<message> content;
        helper::time_point_t delayed_until;
    };

    typedef std::shared_ptr<message> message_ptr;
}

#endif //MDLUTILS_MULTITHREADING_MESSAGES_HPP
