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
    /* Base class for all loopers.
     *
     * It implements break_handler for escaping loop with break messages
     * and implements the logic behind the loop and message handling itself.
     */
    class looper_base : protected break_handler
    {
    protected:
        std::thread::id thread_id;
        std::thread &thread_ref;

        // variable declared mutable to allow const size checks
        mutable std::mutex message_queue_lock;
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
        /* Create new looper instance on a given thread, initializing the set of handlers.
         * @looper_thread Reference to the thread which will run the looper.
         * @handlers References to any number of <mdl::handler>s and at most one mdl::exception_handler.
         */
        template<typename... Handlers>
        looper_base(std::thread &looper_thread, Handlers &... handlers) :
                thread_ref(looper_thread),
                thread_id(looper_thread.get_id()),
                exception_handler(nullptr)
        {
            unpack_handlers(static_cast<mdl::break_handler &>(*this), handlers...);
        }

        /* Virtual destructor. Sends stop signal to the loop, waits until it's done and calls join if the thread
         * is joinable.
         *
         * Warning: may cause infinite loop, if looper is initialized, but loop() method has not been called.
         */
        virtual ~looper_base()
        {
            //std::cout << "Destroying looper_base" << std::endl;
            stop_and_join();
        }

        /* Main function of the looper. Once called, the looper will enter an infinite loop, until stopped by either calling
         * <stop()>, <stop_safely()>, <stop_and_join()>, <stop_and_join_safely()>, or sending <break_message> message.
         */
        void loop();

        /* Waits until the process has been started (<wait_until_started()>) and sets <running> flag false, which should shut
         * the loop down in the next iteration.
         */
        void stop();

        /* Sends break_message to the looper. On the contrary to <stop()>, doesn't wait until looper is started. */
        void stop_safely();

        /* Calls <stop()> method, waits until loop() is finished, and joins the thread, if it's joinable. */
        void stop_and_join();

        /* Calls <stop_safely()> method, waits until loop() is finished, and joins the thread, if it's joinable. */
        void stop_and_join_safely();

        /* Blocks, until looper's started flag is set to true */
        void wait_until_started();

        /* Blocks, until looper's stopped flag is set to true */
        void wait_until_finished();

        /* Evaluates to true, if the looper thread is currently running */
        mdl::get_accessor<bool> running = {
                [&]()
                    {
                        return is_running.load();
                    },
        };

        /* Evaluates to true, if the looper thread has started (but is not necessarily running) */
        mdl::get_accessor<bool> started = {
                [&]()
                    {
                        return is_started.load();
                    },
        };

        /* Evaluates to true, if the looper thread has stopped (but is not necessarily not running) */
        mdl::get_accessor<bool> stopped = {
                [&]()
                    {
                        return is_stopped.load();
                    },
        };

        /* Put the message into the message queue of the looper
         * @msg Shared pointer pointing to the message
         */
        void send_message(message_ptr msg)
        {
            mutex_lock scope_lock(message_queue_lock);
            message_queue.push(msg);
        }

        /* Counts the number of messages in the queue
         *
         * @return Number of messages in the underlying message queue
         */
        size_t count() const
        {
            mutex_lock scope_lock(message_queue_lock);
            return message_queue.size();
        }

        /* Checks whether the message queue is empty.
         *
         * Equivalent to !count().
         *
         * @return true if the message queue is empty, false otherwise.
         */
        bool empty() { return !count(); }
    };

    /* Implementation of <looper_base>, implementing executor_handler and delaying_handler for processing
     * delayed_message and post_call requests.
     */
    class looper : public looper_base, protected delaying_handler, protected executor_handler
    {
    public:
        // @inherit
        template<typename... Handlers>
        looper(std::thread &looper_thread, Handlers &... handlers) :
                looper_base(looper_thread, static_cast<delaying_handler &>(*this),
                            static_cast<executor_handler &>(*this), handlers...),
                delaying_handler(message_queue, message_queue_lock) { }

        // @inherit
        virtual ~looper()
        {
            //std::cout << "Destroying looper" << std::endl;
        }

        /* Put message msg wrapped in delayed_message, to be run at a timestamp equal to
         * std::chrono::high_resolution_clock::now() + duration.
         * @msg Shared pointer to the message instance.
         * @duration Time in duration_t (equivalent to std::chrono::high_resolution_clock::duration)
         *
         * If duration is negative, the message is not delayed, but enqueued immediately.
         */
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

        /* Put message msg wrapped in delayed_message, to be run at timestamp run_at.
         * @msg Shared pointer to the message instance.
         * @run_at Time after which the message should be requeued without the delayed_message wrapper.
         *
         * If run_at is in the past, the message is not delayed, but enqueued immediately.
         */
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

        /* Wrap the function runnable inside a <post_call> message and enqueue it.
         * @runnable A function or callable convertible to std::function, with any return type and without arguments.
         */
        template<typename T>
        void post(std::function<T(void)> runnable)
        {
            send_message(std::make_shared<post_call>(runnable));
        }

        /* Combines <post> and <send_message_delayed>.
         * @runnable A function or callable convertible to std::function, with any return type and without arguments.
         * @duration Time in duration_t (equivalent to std::chrono::high_resolution_clock::duration)
         *
         * If duration is negative, the message is not delayed, but enqueued immediately.
         */
        template<typename T>
        void post_delayed(std::function<T(void)> runnable, duration_t duration)
        {
            send_message_delayed(std::make_shared<post_call>(runnable), duration);
        }


        /* Combines <post> and <send_message_at_time>.
         * @runnable A function or callable convertible to std::function, with any return type and without arguments.
         * @run_at Time after which the message should be requeued without the delayed_message wrapper.
         *
         * If run_at is in the past, the message is not delayed, but enqueued immediately.
         */
        template<typename T>
        void post_at_time(std::function<T(void)> runnable, time_point_t run_at)
        {
            send_message_at_time(std::make_shared<post_call>(runnable), run_at);
        }
    };

    /* Implementation of <looper> associated with std::thread, which automatically invokes the loop() method at startup. */
    class looper_thread : public looper, public std::thread
    {
    public:
        /* Create new looper_thread instance, initializing the set of handlers.
         * @handlers References to any number of <mdl::handler>s and at most one mdl::exception_handler.
         */
        template<typename... Handlers>
        looper_thread(Handlers &... handlers) :
                looper(static_cast<std::thread &>(*this), handlers...),
                std::thread([&]()
                                { loop(); })
        {
            //std::cout << "Created looper_thread" << std::endl;
        }

        // @inherit
        virtual ~looper_thread()
        {
            //std::cout << "Destroying looper_thread" << std::endl;
            if (running || !started)
                stop_and_join();
        }
    };
}

#endif //MDLUTILS_MULTITHREADING_LOOPER_HPP
