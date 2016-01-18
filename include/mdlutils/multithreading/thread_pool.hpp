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
    /* Class providing a pool of workers, which can asynchronously perform selected tasks.
     *
     * Currently, there are two strategies at which the tasks can be assigned and two
     * methods of creating tasks.
     *
     * Strategies: (see <thread_pool::strategy>)
     * Methods: <async>, <map>
     */
    class thread_pool : protected mdl::exception_handler, protected mdl::handler
    {
    public:
        /* Strategies that can be used to assign tasks to workers */
        enum class strategy
        {
            /* In this strategy, each worker performs equal numbers of tasks.
             * Whenever a new task is created, it's posted to the next worker in queue
             * and the worker is requeued.
             * This strategy might cause some workers to be idle for extended periods of time,
             * if their jobs are less time consuming than the others'.
             */
            round_robin,
            /* In this strategy, each worker keeps track of the end of it's message queue.
             * Once it's reached, it asks the <thread_pool> for a new task and becomes temporarily
             * idle, if no tasks are queued in <thread_pool>. This aims at counteracting the situation
             * in which some tasks have long task queues, while others are idle, as the tasks are
             * assigned one-at-a-time.
             */
            dynamic
        };

    protected:
        /* Implementation of <looper_thread> that keeps track of it's parent, and registers him as
         * exception and message handler
         */
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

        // Implementation of <exception_handler> interface. Will enqueue pending exceptions in exception_queue.
        virtual void handle_exception(std::exception_ptr);

        // Implementation of <handler> interface. <thread_pool> keeps track of empty_queue_guard messages in case of
        // dynamic task allocation.
        virtual bool handle_message(mdl::message_ptr);

        // Constructor helper, enqueues empty_queue_guard messages for all workers.
        void initialize_queues();

        typedef mdl::const_vector<thread_handler> pool_type;
        // Pool of all available <thread_handler>s.
        pool_type pool;
        // Iterator to the next handler in case of round_robin task assignment strategy.
        pool_type::iterator next_robin;

        std::mutex exception_queue_lock;
        // Queue holding exception_ptr to all exceptions thrown in the workers.
        std::queue<std::exception_ptr> exception_queue;

        std::mutex task_queue_lock;
        // Queue of messages to be assigned to workers in case of dynamic task assignment.
        std::queue<message_ptr> task_queue;


        void throw_if_nonempty();

        void stop_and_join();

        void send_message(message_ptr);

        unsigned processes;
        strategy task_assigning_strategy;

    public:
        /* Create a <thread_pool> with <processes> workers and <task_assigning_strategy> strategy.
         * @processes Number of worker threads to create. Defaults to std::thread::hardware_concurrency() or 1, if undefined.
         * @task_assigning_strategy Strategy of assigning tasks to the workers. Defaults to strategy::round_robin.
         */
        thread_pool(unsigned processes = helper::hw_concurrency(),
                    strategy task_assigning_strategy = strategy::round_robin) :
                processes(processes),
                task_assigning_strategy(task_assigning_strategy),
                pool(pool_type::make_indexed(processes, *this)),
                next_robin(pool.begin())
        {
            initialize_queues();
        }

        // Destructor. Joins all workers and blocks untill all the tasks are done.
        ~thread_pool(void)
        {
            stop_and_join();
        }

        // The number of workers (threads).
        mdl::const_accessor<unsigned> workers{processes};

        /* Execute the function asynchronously.
         * @fn Function to call.
         * @args... Function arguments.
         *
         * Reassembles the standard function std::async, with few exceptions.
         * First of all, the work will be run on one of the workers in the <thread_pool>.
         * Second, the arguments passed to the function have to be packed inside a lambda expression,
         *   which might cause problems with some complex or uncopyable types, so it's better to
         *   explicitly envelop all references with reference_wrappers, etc.
         *
         * @return std::future<T>, where T is the result type of Fn(Args...), which will hold the
         * result of the fn(args...) once finished.
         */
        template<typename Fn, typename... Args, typename T = typename std::result_of<Fn(Args...)>::type>
        std::future<T>
        async(Fn &&fn, Args &&... args)
        {
            std::shared_ptr<std::promise<T>> promise = std::make_shared<std::promise<T>>();
            std::function<T(typename std::remove_reference<Args>::type...)> floc = fn;
            //std::function<typename std::remove_reference<Fn>::type> floc = fn;
            auto lambda = [promise, floc, args...](void) mutable
                { promise->set_value(floc(std::move(args)...)); };
            send_message(std::make_shared<post_call>(lambda));
            return promise->get_future();
        };

        /* Map all values from one range into another asynchronously.
         * @first Random access iterator pointing to the first element of the range (inkl.).
         * @last Random access iterator pointing to the last element of the range (excl.).
         * @output_first Random access iterator pointing to the first element of the output range.
         * @function Function to call on all elements of range [first, last), which results will
         *  be stored in [output_first, output_first + (last - first))
         *
         * Each task is created with an <async> call. One additional thread is spawned (using std::async)
         * to keep track of promise of completing the map operation.
         *
         * @return std::future that becomes fulfilled once all elements are mapped.
         */
        template<typename RandomAccessIteratorIn, typename RandomAccessIteratorOut, typename Fn>
        std::future<void>
        map(RandomAccessIteratorIn first, RandomAccessIteratorIn last, RandomAccessIteratorOut output_first, Fn function)
        {
            typedef typename std::iterator_traits<RandomAccessIteratorIn>::value_type value_type;
            typedef typename std::iterator_traits<RandomAccessIteratorOut>::value_type result_type;
            typedef typename std::result_of<Fn(value_type)>::type function_result_type;

            static_assert(std::is_convertible<function_result_type, result_type>::value, "Function return type not convertible to iterator value_type");

            // Count the distance between last and first:
            ptrdiff_t n = std::distance(first, last);
            if(n < 0) mdl_throw(make_ia_exception, "Invalid iterator range", "first, last", std::make_pair(first, last));
            // Create n async tasks for all elements in range:
            auto futures = std::make_shared<const_vector<std::future<bool>>>(n);
            for(size_t i = 0; first != last; ++first, ++output_first, ++i)
            {
                futures->at(i) = async([first, output_first, function] ()
                    {
                        *output_first = function(*first);
                        return true;
                    });
            }
            return std::async([futures]()
                                  {
                                      for(auto& job : (*futures))
                                          job.get();
                                      return;
                                  });
        }
    };
}

#endif //MDLUTILS_MULTITHREADING_THREAD_POOL_HPP
