//
// Created by marandil on 09.09.15.
//

#include <mdlutils/multithreading/looper.hpp>
#include <mdlutils/exceptions/break_out_exception.hpp>

namespace mdl
{
    void looper_base::loop()
    {
        std::cout << "Starting looper_base loop()" << std::endl;
        if (running || started)
            mdl_throw(invalid_state_exception<decltype(*this)>, "Looper already running", *this);

        is_running.store(true);
        is_started.store(true);
        is_stopped.store(false);

        std::cout << "Loop marked as started and running" << std::endl;

        try
        {
            while (is_running.load())
            {
                bool queue_empty;
                {
                    mutex_lock scope_lock(message_queue_lock);
                    queue_empty = message_queue.empty();
                }
                if (queue_empty)// wait until there is a job in the queue
                {
                    std::this_thread::yield();
                    continue;
                }

                is_processing.store(true); // mark yourself as currently processing

                message_ptr message;
                try
                {
                    {
                        mutex_lock scope_lock(message_queue_lock);
                        message = message_queue.front();
                        message_queue.pop();
                    }

                    sequential_handle_message(message);
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
        catch (break_out_exception &e)
        {
        }
        is_running.store(false);
        is_stopped.store(true);
    }

    void looper_base::stop()
    {
        wait_until_started();
        is_running.store(false);
    }

    void looper_base::stop_safely()
    {
        send_message(std::make_shared<break_message>());
    }

    void looper_base::stop_and_join()
    {
        wait_until_started();
        stop();
        wait_until_finished();
    }

    void looper_base::stop_and_join_safely()
    {
        wait_until_started();
        stop_safely();
        wait_until_finished();
    }

    void looper_base::sequential_handle_message(message_ptr msg)
    {
        for (auto handler : handler_stack)
        {
            if (handler.get().handle_message(msg))
                return;
        }
    }

    void looper_base::wait_until_started()
    {
        std::cout << "Wait until started..." << std::endl;
        while (!started)
        {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void looper_base::wait_until_finished()
    {
        std::cout << "Wait until finished..." << std::endl;
        while (!stopped)
        {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if(thread_ref.joinable()) thread_ref.join();
    }
}