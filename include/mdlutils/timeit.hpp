//
// Created by marandil on 28.08.15.
//

#ifndef MDLUTILS_TIMEIT_HPP
#define MDLUTILS_TIMEIT_HPP

#include <functional>
#include <chrono>

namespace mdl
{
    /* Standard C++ duration class, corresponding to std::chrono::high_resolution_clock::duration.
     * All instances of timeit function return a value of this type.
     */
    typedef std::chrono::high_resolution_clock::duration timeit_t;

    /* Runs function <function> and measures it's running time. Equivalent to <timeit(function, 1)>
     * @function void() function to measure.
     *
     * @return running time (in timeit_t) of the function <function>.
     */
    timeit_t timeit(std::function<void()> function);

    /* Runs function <function> <count> times and measures it's running time.
     * @function void() function to measure.
     * @count number of repeats.
     *
     * @return running time (in timeit_t) of <count> runs of the function <function>.
     */
    timeit_t timeit(std::function<void()> function, unsigned int count);

    /* Runs function <function>, measures it's running time, and prints the output in a nice way.
     * @function void() function to measure.
     *
     * More or less equivalent to <timeitv(function, 1)>.
     *
     * @return running time (in timeit_t) of the function <function>.
     */
    timeit_t timeitv(std::function<void()> function);

    /* Runs function <function> <count> times, measures it's running time, and prints the output in a nice way.
     * @function void() function to measure.
     * @count number of repeats.
     *
     * More or less equivalent to <timeitv(function, 1)>.
     *
     * @return running time (in timeit_t) of <count> runs of the function <function>.
     */
    timeit_t timeitv(std::function<void()> function, unsigned int count);
}

#endif //MDLUTILS_TIMEIT_HPP
