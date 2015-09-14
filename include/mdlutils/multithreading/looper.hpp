//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_LOOPER_HPP
#define MDLUTILS_MULTITHREADING_LOOPER_HPP

#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <queue>
#include <list>

#include <iostream>

#include <mdlutils/accessor/getset_accessor.hpp>
#include <mdlutils/exceptions/exception_handler.hpp>
#include <mdlutils/exceptions/invalid_state_exception.hpp>
#include <mdlutils/multithreading/handler.hpp>
#include <mdlutils/multithreading/messages.hpp>

namespace mdl
{
    class looper_base : protected break_handler
    {
    protected:
        std::thread::id thread_id;
        std::thread &thread_ref;

        std::mutex message_queue_lock;
        std::queue<message_ptr> message_queue;

        std::atomic_bool is_started{false};
        std::atomic_bool is_running{false};
        std::atomic_bool is_processing{false};
        std::atomic_bool is_stopped{false};

        std::list<std::reference_wrapper<mdl::handler>> handler_stack;

        void sequential_handle_message(message_ptr);

        mdl::exception_handler *exception_handler;

    private:
        void unpack_handlers()
        {
        }

        template<typename Handler, typename... Handlers>
        typename std::enable_if<!std::is_same<Handler, mdl::exception_handler>::value>::type
        unpack_handlers(Handler &handler, Handlers &... rest)
        {
            handler_stack.push_front(handler);
            unpack_handlers(rest...);
        }

        template<typename Handler, typename... Handlers>
        typename std::enable_if<std::is_same<Handler, mdl::exception_handler>::value>::type
        unpack_handlers(Handler &handler, Handlers &... rest)
        {
            exception_handler = &handler;
            unpack_handlers(rest...);
        }

    public:
        template<typename... Handlers>
        looper_base(std::thread &looper_thread, Handlers &... handlers) :
                thread_ref(looper_thread),
                thread_id(looper_thread.get_id()),
                exception_handler(nullptr)
        {
            unpack_handlers(static_cast<mdl::break_handler &>(*this), handlers...);
        }

        virtual ~looper_base()
        {
            std::cout << "Destroying looper_base" << std::endl;
            stop_and_join();
        }

        void loop();

        void stop();

        void stop_safely();

        void stop_and_join();

        void stop_and_join_safely();

        void wait_until_started();
        void wait_until_finished();

        mdl::get_accessor<bool> running = {
                [&]()
                    {
                        return is_running.load();
                    },
        };

        mdl::get_accessor<bool> started = {
                [&]()
                    {
                        return is_started.load();
                    },
        };

        mdl::get_accessor<bool> stopped = {
                [&]()
                    {
                        return is_stopped.load();
                    },
        };

        void send_message(message_ptr msg)
        {
            mutex_lock scope_lock(message_queue_lock);
            message_queue.push(msg);
        }
    };

    class looper : public looper_base, protected delaying_handler, protected executor_handler
    {
    public:
        template<typename... Handlers>
        looper(std::thread &looper_thread, Handlers &... handlers) :
                delaying_handler(message_queue, message_queue_lock),
                looper_base(looper_thread, static_cast<delaying_handler &>(*this),
                            static_cast<executor_handler &>(*this), handlers...) { }

        virtual ~looper()
        {
            std::cout << "Destroying looper" << std::endl;
        }

        void send_message_delayed(message_ptr msg, duration_t duration)
        {
            mutex_lock scope_lock(message_queue_lock);
            if (duration <= duration_t::zero())
                message_queue.push(msg);
            else
                message_queue.push(
                        std::make_shared<delayed_message>(
                                msg, helper::delay_by(duration)
                        )
                );
        }

        void send_message_at_time(message_ptr msg, time_point_t run_at)
        {
            mutex_lock scope_lock(message_queue_lock);
            if (helper::is_after(run_at))
                message_queue.push(msg);
            else
                message_queue.push(
                        std::make_shared<delayed_message>(msg, run_at)
                );
        }

        template<typename T>
        void post(std::function<T(void)> runnable)
        {
            send_message(std::make_shared<post_call>(runnable));
        }

        template<typename T>
        void post_delayed(std::function<T(void)> runnable, duration_t duration)
        {
            send_message_delayed(std::make_shared<post_call>(runnable), duration);
        }

        template<typename T>
        void post_at_time(std::function<T(void)> runnable, time_point_t run_at)
        {
            send_message_at_time(std::make_shared<post_call>(runnable), run_at);
        }
    };

    class looper_thread : public looper, public std::thread
    {
    public:
        template<typename... Handlers>
        looper_thread(Handlers &... handlers) :
                looper(static_cast<std::thread &>(*this), handlers...),
                std::thread([&]()
                                { loop(); })
        {
            std::cout << "Created looper_thread" << std::endl;
        }

        virtual ~looper_thread()
        {
            std::cout << "Destroying looper_thread" << std::endl;
            if (running || !started)
                stop_and_join();
        }
    };
}

#endif //MDLUTILS_MULTITHREADING_LOOPER_HPP
