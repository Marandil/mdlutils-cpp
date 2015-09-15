//
// Created by marandil on 08.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_HELPERS_HPP
#define MDLUTILS_MULTITHREADING_HELPERS_HPP

#include <chrono>
#include <thread>
#include <mutex>

#include <mdlutils/typedefs.hpp>

namespace mdl
{
    typedef std::lock_guard<std::mutex> mutex_lock;

    namespace helper
    {
        unsigned inline hw_concurrency()
        {
            return std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
        }

        time_point_t inline delay_by(duration_t duration)
        {
            return std::chrono::high_resolution_clock::now() + duration;
        }

        bool inline is_after(time_point_t time)
        {
            return (std::chrono::high_resolution_clock::now() > time);
        }
    }
}

#endif //MDLUTILS_MULTITHREADING_HELPERS_HPP
