//
// Created by marandil on 09.09.15.
//

#include <mdlutils/multithreading/handler.hpp>

namespace mdl
{
    void executor_handler::handle_message(message_ptr msg)
    {
        auto msg_post = std::dynamic_pointer_cast<post_call>(msg);
        if (msg_post)
        {
            try
            {
                // invoke the function
                msg_post->function();
            }
            catch (const std::exception &e)
            {
                exception_h.handle_exception(std::current_exception());
            }
        }
    }
}