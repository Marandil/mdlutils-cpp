//
// Created by marandil on 17.08.15.
//

#ifndef MDLUTILS_TYPEDEFS_HPP
#define MDLUTILS_TYPEDEFS_HPP

#include <chrono>
#include <cstdint>
#include <cstddef>

namespace mdl
{
    /* Unsigned integer value representing data stored on a single byte */
    typedef uint8_t byte;

    /* Alias for std::chrono::high_resolution_clock::time_point */
    typedef std::chrono::high_resolution_clock::time_point time_point_t;
    /* Alias for std::chrono::high_resolution_clock::duration */
    typedef std::chrono::high_resolution_clock::duration duration_t;
}

#endif //MDLUTILS_TYPEDEFS_HPP
