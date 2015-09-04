//
// Created by marandil on 04.09.15.
//

#include <mdlutils/bouncing/bounce_manager.hpp>

namespace mdl
{
    bool bounce_manager::eval_bouncer(const bounceable &b)
    {
        bounce_handler *handler = stack.top();
        handler->fun(b);
        return true;
    }

    std::stack<bounce_handler *> bounce_manager::stack;
}