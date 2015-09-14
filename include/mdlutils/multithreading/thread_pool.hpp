//
// Created by marandil on 08.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_THREAD_POOL_HPP
#define MDLUTILS_MULTITHREADING_THREAD_POOL_HPP

#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <future>
#include <vector>
#include <exception>
#include <functional>

#include <mdlutils/exceptions.hpp>
#include <mdlutils/accessor/const_accessor.hpp>
#include <mdlutils/types/const_vector.hpp>
#include <mdlutils/multithreading/helpers.hpp>
#include <mdlutils/multithreading/handler.hpp>
#include <mdlutils/multithreading/looper.hpp>

namespace mdl
{
    class thread_pool : protected mdl::exception_handler, protected mdl::handler
    {
    public:
        enum class strategy
        {
            round_robin,
            dynamic
        };

    protected:
        struct thread_handler : public looper_thread
        {
            thread_pool &parent;
            unsigned id;

            thread_handler(unsigned id, thread_pool &parent) :
                    id(id),
                    parent(parent),
                    looper_thread(static_cast<mdl::exception_handler &>(parent),
                                  static_cast<mdl::handler &>(parent)) { }
        };

        virtual void handle_exception(std::exception_ptr);

        virtual bool handle_message(mdl::message_ptr);

        void initialize_queues();

        typedef mdl::const_vector<thread_handler> pool_type;
        pool_type pool;
        pool_type::iterator next_robin;

        std::mutex exception_queue_lock;
        std::queue<std::exception_ptr> exception_queue;

        std::mutex task_queue_lock;
        std::queue<message_ptr> task_queue;

        void throw_if_nonempty();

        void stop_and_join();

        void send_message(message_ptr);

        unsigned processes;
        strategy task_assigning_strategy;

    public:
        thread_pool(unsigned processes = helper::hw_concurrency(),
                    strategy task_assigning_strategy = strategy::round_robin) :
                processes(processes),
                task_assigning_strategy(task_assigning_strategy),
                pool(pool_type::make_indexed(processes, *this)),
                next_robin(pool.begin())
        {
            initialize_queues();
        }

        ~thread_pool(void)
        {
            stop_and_join();
        }

        mdl::const_accessor<unsigned> workers{processes};

        template<typename Fn, typename... Args, typename T = typename std::result_of<Fn(Args...)>::type>
        std::future<T>
        async(Fn &&fn, Args &&... args)
        {
            std::shared_ptr<std::promise<T>> promise = std::make_shared<std::promise<T>>();
            auto lambda = [promise, fn, args...](void)
                { promise->set_value(fn(args...)); };
            send_message(std::make_shared<post_call>(lambda));
            return promise->get_future();
        };

        template<typename RandomAccessIterator>
        std::future<bool> map(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator output_first)
        {
            // Count the distance between last and first:
            ptrdiff_t n = std::distance(first, last);
            if(n < 0) mdl_throw(make_ia_exception, "Invalid iterator range", "first, last", std::make_pair(first, last));

            return std::async([]() { return true; });
        }
    };
}

#endif //MDLUTILS_MULTITHREADING_THREAD_POOL_HPP
