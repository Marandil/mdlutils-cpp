//
// Created by marandil on 03.09.15.
//

#ifndef MDLUTILS_TYPES_SEQUENCE_ITERATOR_HPP
#define MDLUTILS_TYPES_SEQUENCE_ITERATOR_HPP

#include <mdlutils/exceptions/invalid_argument_exception.hpp>

namespace mdl
{
    template <typename T>
    class sequence_iterator : public std::iterator<std::random_access_iterator_tag, T>
    {
    protected:
        T current, offset;
    public:
        sequence_iterator(T val, T offset = T(1)) : current(val), offset(offset)
        {
            if(offset == T(0))
                mdl_throw(invalid_argument_exception<T>, "Offset's value equals 0", "offset", offset);
        };
        //sequence_iterator(const sequence_iterator<T>& other) : current(other.current), offset(other.offset) { };
        //sequence_iterator(sequence_iterator<T>&& other) : current(std::move(other.current)), offset(std::move(other.offset)) { };

        sequence_iterator<T> &operator++()
        {
            current += offset;
            return *this;
        }

        sequence_iterator<T> operator++(int)
        {
            sequence_iterator<T> tmp(*this);
            operator++();
            return tmp;
        }

        sequence_iterator<T> &operator--()
        {
            current -= offset;
            return *this;
        }

        sequence_iterator<T> operator--(int)
        {
            sequence_iterator<T> tmp(*this);
            operator--();
            return tmp;
        }

        template <typename integer, typename std::enable_if<std::is_integral<integer>::value>::type>
        sequence_iterator<T> operator+=(integer n)
        {
            if(n < 0) return operator-=(-n);
            for(; n > 0; --n) operator++();
            return *this;
        }

        template <typename integer, typename std::enable_if<std::is_integral<integer>::value>::type>
        sequence_iterator<T> operator-=(integer n)
        {
            if(n < 0) return operator+=(-n);
            for(; n > 0; --n) operator--();
            return *this;
        }

        template <typename integer, typename std::enable_if<std::is_integral<integer>::value>::type>
        sequence_iterator<T> operator+(integer n)
        {
            sequence_iterator<T> it(*this);
            return it += n;
        }

        template <typename integer, typename std::enable_if<std::is_integral<integer>::value>::type>
        sequence_iterator<T> operator-(integer n)
        {
            sequence_iterator<T> it(*this);
            return it -= n;
        }

        ptrdiff_t operator-(sequence_iterator<T> b)
        {
            if(b.offset != offset)
                mdl_throw(invalid_argument_exception<T>, "Invalid sequence iterator subtraction - different offsets. Expected " + stringify(offset), "b.offset", b.offset);
            return (current - b.current) / offset;
        }

        bool operator==(const sequence_iterator<T> &rhs) const { return current == rhs.current; }
        bool operator!=(const sequence_iterator<T> &rhs) const { return current != rhs.current; }
		bool operator<(const sequence_iterator<T> &rhs) const
		{
			bool check = current < rhs.current;
			return (offset > 0) ? check : !check;
		}
        bool operator>(const sequence_iterator<T> &rhs) const
		{
			bool check = current > rhs.current;
			return (offset > 0) ? check : !check;
		}
        bool operator<=(const sequence_iterator<T> &rhs) const { return !(*this > rhs); }
        bool operator>=(const sequence_iterator<T> &rhs) const { return !(*this < rhs); }

        T &operator*() { return current; }
        const T &operator*() const { return current; }

        T*operator ->() { return &current; }

        T operator[] (ptrdiff_t n)
        {
            return (*this) + n;
        }

        /*
            Lvalues are swappable.	swap(a,b)
         */

    };

    template <typename T, typename integer, typename std::enable_if<std::is_integral<integer>::value>::type >
    sequence_iterator<T> operator+(integer n, sequence_iterator<T> it)
    {
        return it + n;
    }

    template <typename T, typename integer, typename std::enable_if<std::is_integral<integer>::value>::type >
    sequence_iterator<T> operator-(integer n, sequence_iterator<T> it)
    {
        return it - n;
    }

}

#endif //MDLUTILS_TYPES_SEQUENCE_ITERATOR_HPP
