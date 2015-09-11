//
// Created by marandil on 08.09.15.
//

#include <mdlutils/multithreading/thread_pool.hpp>

namespace mdl
{
    void thread_pool::stop_and_join()
    {
        throw_if_nonempty();

        for (auto &i : pool)
        {
            // TODO: Change to quit_safely once implemented.
            i->running = false;
            i->join();
        }
    }

    void thread_pool::handle_exception(std::exception_ptr ptr)
    {
        std::lock_guard<std::mutex> scope_lock(exception_queue_lock);
        exception_queue.push(ptr);
    }

    void thread_pool::throw_if_nonempty()
    {
        std::lock_guard<std::mutex> scope_lock(exception_queue_lock);
        if (exception_queue.empty()) return;
        std::exception_ptr e = exception_queue.front();
        exception_queue.pop();
        // TODO: (1) when implementing <bounce> mechanics, add checks for bouncable inheritance here and adapt the throw method.
        // TODO: (2) add another exception like rethrown_exception and throw it instead.
        std::rethrow_exception(e);
    }

}