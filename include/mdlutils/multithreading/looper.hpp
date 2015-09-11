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
    class looper_base
    {
    protected:
        std::thread::id thread_id;
        std::thread &thread_ref;

        std::mutex message_queue_lock;
        std::queue<message_ptr> message_queue;

        std::atomic_bool is_running{false};
        std::atomic_bool is_processing{false};
        std::atomic_flag has_empty_queue;

        std::list<std::reference_wrapper<mdl::handler>> handler_stack;
        void sequential_handle_message(message_ptr);

        mdl::exception_handler *exception_handler;

    private:
        void unpack_handlers()
        {
        }

        template<typename Handler, typename... Handlers>
        typename std::enable_if<!std::is_same<Handler, mdl::exception_handler>::value>::type
        unpack_handlers(Handler& handler, Handlers&... rest)
        {
            handler_stack.push_front(handler);
            unpack_handlers(rest...);
        }

        template<typename Handler, typename... Handlers>
        typename std::enable_if<std::is_same<Handler, mdl::exception_handler>::value>::type
        unpack_handlers(mdl::exception_handler& handler, Handlers&... rest)
        {
            exception_handler = &handler;
            unpack_handlers(rest...);
        }

    public:
        template <typename... Handlers>
        looper_base(std::thread &looper_thread, Handlers&... handlers) :
                thread_ref(looper_thread),
                thread_id(looper_thread.get_id()),
                exception_handler(nullptr)
        {
            unpack_handlers(handlers...);
        }

        virtual ~looper_base()
        {
            std::cout << "Destroying looper_base" << std::endl;
            is_running.store(false);
        }

        void loop();
        void stop();

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
    };

    class looper : public looper_base, public delaying_handler, public executor_handler
    {
    public:
        template <typename... Handlers>
        looper(std::thread &looper_thread, Handlers&... handlers) :
                delaying_handler(message_queue, message_queue_lock),
                looper_base(looper_thread, static_cast<delaying_handler&>(*this), static_cast<executor_handler&>(*this), handlers...)
        { }

        virtual ~looper()
        {
            std::cout << "Destroying looper" << std::endl;
        }


        template<typename T>
        void post(std::function<T(void)> runnable)
        {
            mutex_lock scope_lock(message_queue_lock);
            message_queue.push(std::make_shared<post_call>(runnable));
        }

        template<typename T>
        void post_delayed(std::function<T(void)> runnable, duration_t duration)
        {
            mutex_lock scope_lock(message_queue_lock);
            if (duration <= duration_t::zero())
                message_queue.push(std::make_shared<post_call>(runnable));
            else
                message_queue.push(
                        std::make_shared<delayed_message>(
                                std::make_shared<post_call>(runnable),
                                helper::delay_by(duration)
                        )
                );
        }

        template<typename T>
        void post_at_time(std::function<T(void)> runnable, time_point_t run_at)
        {
            mutex_lock scope_lock(message_queue_lock);
            if (helper::is_after(run_at))
                message_queue.push(std::make_shared<post_call>(runnable));
            else
                message_queue.push(
                        std::make_shared<delayed_message>(
                                std::make_shared<post_call>(runnable),
                                run_at
                        )
                );
        }
    };

    class looper_thread : public looper, public std::thread
    {
    public:
        template <typename... Handlers>
        looper_thread(Handlers&... handlers) :
                looper(static_cast<std::thread&>(*this), handlers...),
                std::thread([&](){loop();})
        {
            std::cout << "Created looper_thread" << std::endl;
        }

        virtual ~looper_thread()
        {
            std::cout << "Destroying looper_thread" << std::endl;
            stop();
            if(joinable())
                join();
            else
                mdl_throw(invalid_state_exception<looper_thread>, "Destroying detached and running looper_thread", *this)
        }

    };
}

#endif //MDLUTILS_MULTITHREADING_LOOPER_HPP
