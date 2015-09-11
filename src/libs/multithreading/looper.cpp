//
// Created by marandil on 09.09.15.
//

#include <mdlutils/multithreading/looper.hpp>

namespace mdl
{
    void looper::loop()
    {
        has_empty_queue.test_and_set(); // we want this to be 1 by default
        while (is_running.load())
        {
            if (has_empty_queue.test_and_set()) // wait until there is a job in the queue
            {
                std::this_thread::yield();
                continue;
            }

            is_processing.store(true); // mark yourself as currently processing

            try
            {
                message_ptr message;
                {
                    std::lock_guard<std::mutex> scope_lock(message_queue_lock);
                    message = message_queue.front();
                    message_queue.pop();
                }

                handler.handle_message(message);
            }
            catch (std::exception &e)
            {
                std::exception_ptr p = std::current_exception();
                if (exception_handler)
                    exception_handler->handle_exception(p);
                else
                    std::rethrow_exception(p);
            }

            is_processing.store(false);
        }
    }
}