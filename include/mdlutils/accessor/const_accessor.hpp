//
// Created by marandil on 04.09.15.
//

#ifndef MDLUTILS_ACCESSOR_CONST_ACCESSOR_HPP
#define MDLUTILS_ACCESSOR_CONST_ACCESSOR_HPP

namespace mdl
{
    template<typename T>
    class const_accessor
    {
    private:
        typedef const_accessor<T> thistype;
    protected:
        const T &reference;

    public:
        const_accessor(const T &ref)
                : reference(ref) { }

        // Explicitly deleted default constructor
        const_accessor() = delete;

        // Explicitly deleted copy constructor
        const_accessor(const thistype &) = delete;

        operator const T &() const
        {
            return reference;
        }
    };

    template<typename T> using get_accessor = const_accessor<T>;

}

#endif //MDLUTILS_CPP_CONST_ACCESSOR_HPP
