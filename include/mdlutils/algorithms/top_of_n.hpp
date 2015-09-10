//
// Created by marandil on 18.08.15.
//

#ifndef MDLUTILS_ALGORITHMS_TOP_OF_N_HPP
#define MDLUTILS_ALGORITHMS_TOP_OF_N_HPP

#include <functional>
#include <algorithm>
#include <vector>
#include <list>
#include <set>

#include <mdlutils/exceptions/invalid_argument_exception.hpp>
#include <mdlutils/types/range.hpp>
#include <mdlutils/types/sorted_list.hpp>

namespace mdl
{
    template<typename RandomAccessIterator, typename Compare>
    inline std::list<std::reference_wrapper<typename RandomAccessIterator::value_type>>
    top_of_n_list(RandomAccessIterator first, RandomAccessIterator last, size_t elements,
                  Compare comp)
    {
        typedef std::reference_wrapper<typename RandomAccessIterator::value_type> ref_t;
        typedef std::list<ref_t> result_t;
        typedef mdl::sorted_list<ref_t, Compare> sorted_list;

        if (last - first < elements)
        {
            mdl_throw(invalid_argument_exception<size_t>,
                      "Number of elements greater than range size (" + std::to_string(last - first) + ")", "elements",
                      elements);
        }
        sorted_list temporary;

        for (size_t i : range<size_t>(elements))
            temporary.insert(*(first++));

        for (; first != last; ++first)
        {
            if (comp(temporary.back(), *first)) continue;
            temporary.insert(*first);
            temporary.pop_back();
        }

        return static_cast<result_t>(temporary); // should use sorted_list explicit list() operator
    }

    template<typename RandomAccessIterator, typename Compare>
    inline std::multiset<std::reference_wrapper<typename RandomAccessIterator::value_type>, Compare>
    top_of_n_set(RandomAccessIterator first, RandomAccessIterator last, size_t elements,
                 Compare comp)
    {
        if (last - first < elements)
        {
            mdl_throw(invalid_argument_exception<size_t>,
                      "Number of elements greater than range size (" + std::to_string(last - first) + ")", "elements",
                      elements);
        }
        std::multiset<std::reference_wrapper<typename RandomAccessIterator::value_type>, Compare> temporary(comp);
        for (size_t _ : range<size_t>(elements))
        {
            temporary.insert((*first));
            ++first;
        }
        for (; first != last; ++first)
        {
            if (comp(*temporary.rbegin(), *first)) continue;
            temporary.insert((*first));
            temporary.erase(--temporary.end());
        }
        return temporary;
    }

    template<typename RandomAccessIterator, typename Compare>
    inline std::vector<std::reference_wrapper<typename RandomAccessIterator::value_type>>
    top_of_n_vector(RandomAccessIterator first, RandomAccessIterator last, size_t elements,
                    Compare comp)
    {
        if (last - first < elements)
        {
            mdl_throw(invalid_argument_exception<size_t>,
                      "Number of elements greater than range size (" + std::to_string(last - first) + ")", "elements",
                      elements);
        }

        typedef std::reference_wrapper<typename RandomAccessIterator::value_type> ref_t;
        typedef std::vector<ref_t> result_t;

        auto h_comp = [&comp](ref_t a, ref_t b)
            { return !comp(a, b); }; // The comparator has to be negated, as we aim for std::greater here

        result_t heap(first, last);
        typename result_t::iterator h_begin = heap.begin(), h_end = heap.end();
        std::make_heap(h_begin, h_end, h_comp);

        result_t temporary;
        temporary.reserve(elements); // Can't resize for references

        for (size_t _ : range<size_t>(elements))
        {
            temporary.push_back(heap.front());
            std::pop_heap(h_begin, h_end--, h_comp); // pop heap without unnecessary overhead
        }

        return temporary;
    }
}

#endif //MDLUTILS_ALGORITHMS_TOP_OF_N_HPP
