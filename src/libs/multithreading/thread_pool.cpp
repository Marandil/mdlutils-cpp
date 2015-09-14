//
// Created by marandil on 08.09.15.
//

#include <mdlutils/multithreading/thread_pool.hpp>
#include <mdlutils/types/range.hpp>

namespace mdl
{
    struct empty_queue_guard : public message
    {
        unsigned queue_id;

        empty_queue_guard(unsigned id) : queue_id(id) { }

        virtual ~empty_queue_guard() { }
    };

    bool thread_pool::handle_message(message_ptr msg)
    {
        auto msg_queue_guard = std::dynamic_pointer_cast<empty_queue_guard>(msg);
        if (msg_queue_guard)
        {
            if (pool[msg_queue_guard->queue_id].empty())
            {
                // TODO: logic for empty message queue
                if (task_assigning_strategy == strategy::dynamic)
                {
                    message_ptr task = nullptr;
                    {
                        mutex_lock scope_lock(task_queue_lock);
                        if (!task_queue.empty())
                        {
                            task = task_queue.front();
                            task_queue.pop();
                        }
                    }
                    if (task != nullptr)
                    {
                        pool[msg_queue_guard->queue_id].send_message(task);
                    }
                    else
                    {
                        std::this_thread::yield();
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                }
                else
                {
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            // Resend empty queue guard
            pool[msg_queue_guard->queue_id].send_message(msg);
            return true;
        }
        return false;
    }

    void thread_pool::send_message(message_ptr msg)
    {
        switch (task_assigning_strategy)
        {
            case strategy::dynamic:
            {
                mutex_lock scope_lock(task_queue_lock);
                task_queue.push(msg);
                break;
            }
            case strategy::round_robin:
            {
                next_robin->send_message(msg);
                ++next_robin;
                if (next_robin == pool.end()) next_robin = pool.begin();
                break;
            };
        }
    }

    void thread_pool::initialize_queues()
    {
        for (unsigned i : range<unsigned>(processes))
            pool[i].send_message(std::make_shared<empty_queue_guard>(i));
    }

    void thread_pool::stop_and_join()
    {
        throw_if_nonempty();
        for (auto &i : pool)
        {
            i.stop_and_join_safely();
            throw_if_nonempty();
        }
    }

    void thread_pool::handle_exception(std::exception_ptr ptr)
    {
        mutex_lock scope_lock(exception_queue_lock);
        exception_queue.push(ptr);
    }

    void thread_pool::throw_if_nonempty()
    {
        std::exception_ptr e;
        {
            mutex_lock scope_lock(exception_queue_lock);
            if (exception_queue.empty())
                return;
            e = exception_queue.front();
            exception_queue.pop();
        }
        // TODO: (1) when implementing <bounce> mechanics, add checks for bouncable inheritance here and adapt the throw method.
        // TODO: (2) add another exception like rethrown_exception and throw it instead.
        std::rethrow_exception(e);
    }

}