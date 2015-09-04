//
// Created by marandil on 04.09.15.
//

#ifndef MDLUTILS_CPP_BOUNCE_MANAGER_HPP
#define MDLUTILS_CPP_BOUNCE_MANAGER_HPP

#include <stack>
#include <functional>

namespace mdl
{

    class bounceable
    {

    };

    class bounce_handler
    {
    public:
        std::function<void(const bounceable &)> fun;
        bounce_handler(std::function<void(const bounceable &)> a);
        ~bounce_handler();
    };

    class bounce_manager
    {
    private:
        static std::stack<bounce_handler *> stack;
    public:
        static bool empty() { return stack.empty(); }

        static bool eval_bouncer(const bounceable &b);
    };
}


#endif //MDLUTILS_CPP_BOUNCE_MANAGER_HPP
