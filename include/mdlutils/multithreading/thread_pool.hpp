//
// Created by marandil on 08.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_THREAD_POOL_HPP
#define MDLUTILS_MULTITHREADING_THREAD_POOL_HPP

#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <vector>
#include <exception>
#include <functional>

#include <mdlutils/exceptions.hpp>
#include <mdlutils/multithreading/helpers.hpp>

namespace mdl
{
    class thread_pool
    {
    protected:
        struct thread_handler
        {
            thread_pool& parent;
            unsigned id;

            thread_handler(thread_pool& parent, unsigned id) : id(id), parent(parent), thread([&]{this->operator()();}) { }

            std::thread thread;
            std::atomic_bool is_running{true};
            std::atomic_bool is_processing{false};
            std::atomic_flag has_empty_queue; // defaults to false?

            std::function<void()> job;

            void operator() ();
        };

        std::vector<std::shared_ptr<thread_handler>> pool;

        std::mutex exception_queue_lock;
        std::queue<std::exception_ptr> exception_queue;

        void register_exception(std::exception_ptr ptr);

        void throw_if_nonempty();

        unsigned processes;

    public:
        thread_pool(unsigned processes = helper::hw_concurrency()) : processes(processes), pool(processes, nullptr)
        {
            while(processes--) pool[processes].reset(new thread_handler(*this, processes));
        }

        ~thread_pool(void)      {    }
    };
}

#endif //MDLUTILS_MULTITHREADING_THREAD_POOL_HPP
