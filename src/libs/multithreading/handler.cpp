//
// Created by marandil on 09.09.15.
//

#include <mdlutils/multithreading/handler.hpp>
#include <mdlutils/exceptions/not_implemented_exception.hpp>

namespace mdl
{
    bool executor_handler::handle_message(message_ptr msg)
    {
        auto msg_post = std::dynamic_pointer_cast<post_call>(msg);
        if (msg_post)
        {
            // invoke the function
            msg_post->function();
            return true;
        }
        return false;
    }

    bool delaying_handler::handle_message(message_ptr msg)
    {
        // TODO: Implement this
        mdl_throw(not_implemented_exception, "");
        return false;
    }
}