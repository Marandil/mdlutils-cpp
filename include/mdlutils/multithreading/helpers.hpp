//
// Created by marandil on 08.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_HELPERS_HPP
#define MDLUTILS_MULTITHREADING_HELPERS_HPP

namespace mdl
{
    namespace helper
    {
        unsigned hw_concurrency()
        {
            return std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
        }
    }
}

#endif //MDLUTILS_MULTITHREADING_HELPERS_HPP
