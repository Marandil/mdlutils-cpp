//
// Created by marandil on 28.08.15.
//

#ifndef MDLUTILS_TIMEIT_HPP
#define MDLUTILS_TIMEIT_HPP

#include <functional>
#include <chrono>

namespace mdl
{
    typedef std::chrono::high_resolution_clock::duration timeit_t;

    timeit_t timeit(std::function<void()> function);

    timeit_t timeit(std::function<void()> function, unsigned int count);

    timeit_t timeitv(std::function<void()> function);

    timeit_t timeitv(std::function<void()> function, unsigned int count);
}

#endif //MDLUTILS_TIMEIT_HPP
