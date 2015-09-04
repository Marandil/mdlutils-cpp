//
// Created by marandil on 04.09.15.
//

#ifndef MDLUTILS_ACCESSORS_SIMPLE_ACCESSOR_HPP
#define MDLUTILS_ACCESSORS_SIMPLE_ACCESSOR_HPP

#include <functional>

namespace mdl
{

    template<typename T>
    class simple_accessor
    {
    private:
        typedef simple_accessor<T> thistype;
    protected:
        T &reference;

    public:
        simple_accessor(T &ref)
                : reference(ref) { }

        // Explicitly deleted default constructor
        simple_accessor() = delete;

        // Explicitly deleted copy constructor
        simple_accessor(const thistype &) = delete;

        operator const T &() const
        {
            return reference;
        }

        template<typename V>
        const thistype &operator=(V op) const
        {
            reference = static_cast<const T &>(op); // utilizes T&() operator of op
            return *this;
        }


        const thistype &operator=(const thistype &op) const
        {
            // Apparently, this operator does not implicitly goes into the template argument list
            return this->operator=<const thistype &>(op);
        }

        template<typename V>
        const thistype &operator+=(const V &op) const
        {
            reference += static_cast<const T>(op); // utilizes T&() operator of op
            return *this;
        }

        template<typename V>
        const thistype &operator-=(const V &op) const
        {
            reference -= static_cast<const T>(op); // utilizes T&() operator of op
            return *this;
        }

        template<typename V>
        const thistype &operator*=(const V &op) const
        {
            reference *= static_cast<const T>(op); // utilizes T&() operator of op
            return *this;
        }

        template<typename V>
        const thistype &operator/=(const V &op) const
        {
            reference /= static_cast<const T>(op); // utilizes T&() operator of op
            return *this;
        }
    };

}

#endif //MDLUTILS_ACCESSORS_SIMPLE_ACCESSOR_HPP
