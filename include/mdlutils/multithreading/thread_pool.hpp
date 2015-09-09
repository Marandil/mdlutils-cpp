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
#include <mdlutils/accessor/const_accessor.hpp>
#include <mdlutils/multithreading/helpers.hpp>
#include <mdlutils/multithreading/handler.hpp>
#include <mdlutils/multithreading/looper.hpp>

namespace mdl
{
    class thread_pool
    {
    protected:
        struct thread_handler : public looper_thread, public mdl::handler
        {
            thread_pool &parent;
            unsigned id;

            thread_handler(thread_pool &parent, unsigned id) :
                    id(id),
                    parent(parent),
                    mdl::handler(), looper_thread(static_cast<mdl::handler&>(*this))
            { }

            virtual void handle_message(message_ptr msg);
        };

        std::vector<std::shared_ptr<thread_handler>> pool;

        std::mutex exception_queue_lock;
        std::queue<std::exception_ptr> exception_queue;

        void register_exception(std::exception_ptr ptr);

        void throw_if_nonempty();

        void stop_and_join();

        unsigned processes;

    public:
        thread_pool(unsigned processes = helper::hw_concurrency()) : processes(processes), pool(processes, nullptr)
        {
            while (processes--) pool[processes].reset(new thread_handler(*this, processes));
        }

        ~thread_pool(void)
        {
            stop_and_join();
        }

        mdl::const_accessor<unsigned> workers{processes};
    };
}

#endif //MDLUTILS_MULTITHREADING_THREAD_POOL_HPP
