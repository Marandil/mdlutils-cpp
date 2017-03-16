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
            msg_post->function = nullptr;
            msg_post = nullptr;
            return true;
        }
        return false;
    }

    bool delaying_handler::handle_message(message_ptr msg)
    {
        auto msg_delay = std::dynamic_pointer_cast<delayed_message>(msg);
        if (msg_delay)
        {
            // If it's already after msg->delayed_until, push the content of the message
            if(helper::is_after(msg_delay->delayed_until))
                queue.push(msg_delay->content);
            // Repush delayed message otherwise
            else
                queue.push(msg_delay);
            return true;
        }
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