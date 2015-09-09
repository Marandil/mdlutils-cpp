//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_MESSAGES_HPP
#define MDLUTILS_MULTITHREADING_MESSAGES_HPP

#include <memory>
#include <functional>

namespace mdl
{
    struct message { };

    struct post_call
    {
        std::function<void(void)> function;
    };

    typedef std::shared_ptr<message> message_ptr;
}

#endif //MDLUTILS_MULTITHREADING_MESSAGES_HPP
