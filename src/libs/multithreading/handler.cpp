//
// Created by marandil on 09.09.15.
//

#include <mdlutils/multithreading/handler.hpp>
#include <mdlutils/exceptions/break_out_exception.hpp>
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

    bool break_handler::handle_message(message_ptr msg)
    {
        auto msg_break = std::dynamic_pointer_cast<break_message>(msg);
        if (msg_break)
        {
            throw break_out_exception{}; // We don't use mdl_throw here, since we don't want it's mechanics.
        }
        return false;
    }
}