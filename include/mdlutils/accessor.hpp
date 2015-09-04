//
// Created by marandil on 18.08.15.
//

#ifndef MDLUTILS_ACCESSOR_HPP
#define MDLUTILS_ACCESSOR_HPP

#include <mdlutils/accessors/simple_accessor.hpp>
#include <mdlutils/accessors/const_accessor.hpp>

namespace mdl
{

    template<typename T>
    class set_accessor : public simple_accessor<T>
    {
    private:
        typedef set_accessor<T> thistype;

    public:
        set_accessor(T& ref)
                : simple_accessor<T>(ref)
        {}
        operator const T&() const = delete;
    };
}

#endif //MDLUTILS_ACCESSOR_HPP
