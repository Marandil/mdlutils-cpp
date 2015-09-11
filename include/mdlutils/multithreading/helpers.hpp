//
// Created by marandil on 08.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_HELPERS_HPP
#define MDLUTILS_MULTITHREADING_HELPERS_HPP

#include <chrono>
#include <thread>

namespace mdl
{
    namespace helper
    {
        unsigned inline hw_concurrency()
        {
            return std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
        }

        typedef std::chrono::high_resolution_clock::time_point time_point_t;
        typedef std::chrono::high_resolution_clock::duration duration_t;

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
