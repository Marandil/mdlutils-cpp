//
// Created by marandil on 08.09.15.
//

#include <mdlutils/multithreading/thread_pool.hpp>

namespace mdl
{
    void thread_pool::thread_handler::operator()()
    {
        has_empty_queue.test_and_set(); // we want this to be 1 by default
        while (is_running.load())
        {
            while (has_empty_queue.test_and_set()) // wait until there is a job in the queue
                std::this_thread::yield();
            is_processing.store(true); // mark yourself as currently processing
            std::function<void()> current = job; // retrieve the function pointer to a local variable
            if (!job) // if the retrieved job is null, throw invalid_argument_exception
                mdl_throw(mdl::make_ia_exception, "job", nullptr);
            job = nullptr; // set the job to be null,
            // pool should check whether nothing is assigned, even if the thread claims to not be processing
            // (is_processing == false with a job in queue is possible)

            try
            {
                // invoke the job
                current();
                // after the job has been done
            }
            catch (const std::exception &e)
            {
                parent.register_exception(std::current_exception());
            }

            is_processing.store(false);
        }
    }

    void thread_pool::register_exception(std::exception_ptr ptr)
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