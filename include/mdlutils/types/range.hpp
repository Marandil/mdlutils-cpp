//
// Created by marandil on 28.08.15.
//

#ifndef MDLUTILS_TYPES_RANGE_HPP
#define MDLUTILS_TYPES_RANGE_HPP

#include <iterator>

#include <mdlutils/types/sequence_iterator.hpp>

namespace mdl
{

    /* A simple C++-like iterator for python-style range(...) iterable
     */
    template<typename T>
    class range
    {
    private:
        T begin_val;
        T offset;
        T end_val;
    public:
        /* Create a range [0, last) with step 1 */
        range(const T &last) : begin_val(0), offset(1), end_val(last) { }

        /* Create a range [first, last) with step 1 */
        range(const T &first, const T &last) : begin_val(first), offset(1), end_val(last) { }

        /* Create a range [first, last) with step offset */
        range(const T &first, const T &last, const T &offset) : begin_val(first), offset(offset), end_val(last) { }


        typedef mdl::sequence_iterator<T> iterator;
        typedef iterator const_iterator;
        typedef iterator reverse_iterator;
        typedef iterator const_reverse_iterator;
        typedef T value_type;
        typedef T &reference;
        typedef const T &const_reference;

        iterator begin() const { return iterator(begin_val, offset); }

        iterator end() const
        {
            //T end = ((end_val - begin_val + offset - T(1)) / offset) * offset;
            if((end_val > begin_val && offset > 0) || (end_val < begin_val && offset < 0))
            {
                T end_offset = (end_val - begin_val) % offset;
                if (end_offset == T(0))
                    return iterator(end_val, offset);
                else
                    return iterator(end_val + offset - end_offset, offset);
            }
            else return iterator(begin_val, offset); // end == begin for empty (or negative) range
        }

        iterator cbegin() const { return begin(); }

        iterator cend() const { return end(); }

        size_t size() const { return (end_val - begin_val) / offset; }
    };
}

#endif //MDLUTILS_TYPES_RANGE_HPP
