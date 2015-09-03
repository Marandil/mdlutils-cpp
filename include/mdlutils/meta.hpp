//
// Created by marandil on 03.09.15.
//

#ifndef MDLUTILS_META_HPP
#define MDLUTILS_META_HPP

// This header includes all other headers to ensure compatiblity and ease includes in some cases.
// Warning: It should not be used in production code, as it may include a lot of unnecessary (extern) symbols.

#include <mdlutils/exceptions.hpp>
//#include <mdlutils/algorithms.hpp> (TBA)
#include <mdlutils/typedefs.hpp>
#include <mdlutils/timeit.hpp>

// Types (individual)

#include <mdlutils/types/sequence_iterator.hpp>
#include <mdlutils/types/sorted_list.hpp>

// Exceptions (should have been already included via exceptions.hpp)

#include <mdlutils/exceptions/base_exception.hpp>
#include <mdlutils/exceptions/invalid_argument_exception.hpp>
#include <mdlutils/exceptions/not_implemented_exception.hpp>


#endif //MDLUTILS_META_HPP
