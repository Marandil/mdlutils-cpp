//
// Created by marandil on 28.08.15.
//

#include <iostream>
#include <iomanip>

#include <mdlutils/timeit.hpp>

namespace mdl
{
    timeit_t timeit(std::function<void()> function)
    {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        function();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        return (stop - start);
    }

    timeit_t timeit(std::function<void()> function, unsigned int repeats)
    {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        while (repeats--)
            function();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        return (stop - start);
    }

    timeit_t timeitv(std::function<void()> function)
    {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        function();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        timeit_t duration = (stop - start);
        uint64_t s, ms, us, ns;
        s = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
        us = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000;
        ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000;
        std::cout << s << "." <<
        std::setfill('0') << std::setw(3) << ms << "'" <<
        std::setfill('0') << std::setw(3) << us << "'" <<
        std::setfill('0') << std::setw(3) << ns << std::endl;
        return duration;
    }

    timeit_t timeitv(std::function<void()> function, unsigned int repeats)
    {
        unsigned int r = repeats;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        while (r--)
            function();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        timeit_t duration = (stop - start);
        uint64_t s, ms, us, ns;
        s = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
        us = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000;
        ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000;
        std::cout << repeats << " runs : " << s << "." <<
        std::setfill('0') << std::setw(3) << ms << "'" <<
        std::setfill('0') << std::setw(3) << us << "'" <<
        std::setfill('0') << std::setw(3) << ns << std::endl;
        return duration;
    }
}
