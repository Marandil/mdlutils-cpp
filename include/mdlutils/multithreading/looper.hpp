//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_LOOPER_HPP
#define MDLUTILS_MULTITHREADING_LOOPER_HPP

#include <thread>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>

#include <mdlutils/accessor/getset_accessor.hpp>
#include <mdlutils/exceptions/exception_handler.hpp>
#include <mdlutils/multithreading/handler.hpp>
#include <mdlutils/multithreading/messages.hpp>

namespace mdl
{
    class looper
    {
    protected:
        std::thread::id thread_id;
        std::thread &thread_ref;

        std::mutex message_queue_lock;
        std::queue<message_ptr> message_queue;

        std::atomic_bool is_running{true};
        std::atomic_bool is_processing{false};
        std::atomic_flag has_empty_queue;

        mdl::handler &handler;
        mdl::exception_handler *exception_handler;

    public:
        looper(std::thread &looper_thread, mdl::handler &handler) :
                handler(handler),
                thread_ref(looper_thread),
                thread_id(looper_thread.get_id()),
                exception_handler(nullptr) { }

        looper(std::thread &looper_thread, mdl::handler &handler, mdl::exception_handler &exception_handler) :
                handler(handler),
                thread_ref(looper_thread),
                thread_id(looper_thread.get_id()),
                exception_handler(&exception_handler) { }

        void loop();

        mdl::getset_accessor<bool> running = {
                [&]()
                    {
                        return is_running.load();
                    },
                [&](bool value)
                    {
                        is_running.store(value);
                    }
        };

        template<typename T>
        void post(std::function<T(void)> runnable)
        {
            std::lock_guard<std::mutex> scope_lock(message_queue_lock);
            message_queue.push(std::make_shared<post_call>(runnable));
        }
    };

    class looper_thread : public looper, public std::thread
    {
    public:
        looper_thread(mdl::handler& handler) :
                std::thread([&](){loop();}),
                looper(static_cast<std::thread&>(*this), handler) { }
    };
}

#endif //MDLUTILS_MULTITHREADING_LOOPER_HPP
