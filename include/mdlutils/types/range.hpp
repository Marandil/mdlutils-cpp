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

        struct end_val_calculator {
            end_val_calculator(const T &first, const T &last, const T &offset){
                if((last > first && offset > 0) || (last < first && offset < 0))
                {
                    T end_offset = (last - first) % offset;
                    if (end_offset == T(0))
                        value = last;
                    else
                        value = last + offset - end_offset;
                }
                else  value = first; // end == begin for empty (or negative) range
            }

            T value;
        };

        end_val_calculator end_calc;

    public:
        /* Create a range [0, <last>) with step 1
         * @last The element after the last element in the range (e.g. if <last> = 5, the range contains numbers {0, 1, 2, 3, 4}
         */
        range(const T &last) : begin_val(0), offset(1), end_calc(begin_val, last, offset) { }

        /* Create a range [<first>, <last>) with step 1
         * @first The first element in the range.
         * @last The element after the last element in the range (the ranges are exclusive).
         */
        range(const T &first, const T &last) : begin_val(first), offset(1), end_calc(begin_val, last, offset) { }

        /* Create a range [<first>, <last>) with step <offset>.
         * @first The first element in the range.
         * @last The element after the last element in the range (the ranges are exclusive).
         * @offset The difference between consecutive elements in the range.
         *
         * Note, that last will not always be the same as the value of <end()> iterator. <end()> will always be >= to @last
         * if <offset> > 0 and <= <last> if <offset> < 0. <offset> = 0 will throw invalid_argument_exception.
         */
        range(const T &first, const T &last, const T &offset) : begin_val(first), offset(offset), end_calc(begin_val, last, offset)
        {
            if(offset == T(0))
                mdl_throw(invalid_argument_exception<T>, "Offset's value equals 0", "offset", offset);
        }

        // A  random access iterator (<sequence_iterator>) to T.
        typedef mdl::sequence_iterator<T> iterator;
        // A random access iterator (<sequence_iterator>) to T.
        typedef iterator const_iterator;
        // A random access iterator (<sequence_iterator>) to T with negative offset.
        typedef iterator reverse_iterator;
        // A random access iterator (<sequence_iterator>) to T with negative offset.
        typedef iterator const_reverse_iterator;
        // The template parameter T.
        typedef T value_type;
        // The reference to the template parameter T (T&).
        typedef T &reference;
        // The const reference to the template parameter T (const T&).
        typedef const T &const_reference;

        /* Return iterator to beginning of the range.
         *
         * @return <sequence_iterator> with (first and offset) parameters of the range.
         */
        iterator begin() const { return iterator(begin_val, offset); }

        /* Return iterator to end of the range.
         *
         * The resulting iterator is not the last element of the range, but rather the first item that would appear in
         * the range that's >= than last if offset > 0 or <= if offset < 0.
         *
         * @return <sequence_iterator> with the parameters of the range.
         */
        iterator end() const { return iterator(end_calc.value, offset); }

        /* See <begin>. */
        iterator cbegin() const { return begin(); }

        /* See <end>. */
        iterator cend() const { return end(); }


        /* Return number of elements in range.
         *
         * @return Number of elements in range.
         */
        size_t size() const
        {
            if((end_calc.value > begin_val && offset < 0) || (end_calc.value < begin_val && offset > 0))
                return 0;
            return (end_calc.value - begin_val) / offset;
        }
    };
}

#endif //MDLUTILS_TYPES_RANGE_HPP
