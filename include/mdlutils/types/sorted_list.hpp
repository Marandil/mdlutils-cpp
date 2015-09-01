//
// Created by marandil on 28.08.15.
//

#ifndef MDLUTILS_SORTED_LIST_HPP
#define MDLUTILS_SORTED_LIST_HPP

#include <list>
#include <functional>

namespace mdl
{
    template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    class sorted_list
    {
    protected:
        typedef std::list<T, Alloc> base_type;
        base_type base;
        Compare comp;
    public:
        typedef typename base_type::iterator iterator;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::reverse_iterator reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;

        sorted_list(Compare comp = Compare()) : base(), comp(comp) { }

        /* Insert element
         *
         * @value the value to be inserted.
         * @return an iterator that points to the inserted element.
         */
        iterator insert(const T& value)
        {
            iterator it = base.begin(), end = base.end();

            if(it == end) // if the container is empty
                return base.insert(it, value);

            if(comp(base.back(), value)) // shortcut, if the element is greater than the last element in the list
                return base.insert(end, value);

            for(; it != end; ++it)
                if(comp(value, *it)) break;
            return base.insert(it, value);
        }

        /* Insert element, given an hinted position
         *
         * @position hinted position to consider while inserting
         * @value the value to be inserted.
         * @return an iterator that points to the inserted element.
         */
        iterator insert(iterator position, const T& value)
        {
            iterator begin = base.begin(), end = base.end(), it = position;

            if(begin == end) // if the container is empty
                return base.insert(begin, value);

            if(comp(base.back(), value)) // shortcut, if the element is greater than the last element in the list
                return base.insert(end, value);
            if(comp(value, base.front())) // shortcut, if the element is smaller than the first element in the list
                return base.insert(begin, value);

            if(comp(value, *it)) // the value is smaller than the hinted position, therefore insert it to the left:
            {
                for (; it != begin; --it)
                    if (comp(*it, value)) break;
                return base.insert(++it, value); // we need to insert at an element greater than the smaller one
            }
            else // the value is greater-or-equal than the hinted position, therefore insert it to the right:
            {
                for (; it != end; ++it)
                    if (comp(value, *it)) break;
                return base.insert(it, value);
            }
        }

        /* Insert range of elements
         *
         * @first iterator specifying the first element in the inserting range
         * @last iterator specifying the last element in the inserting range
         * @return an iterator that points to the last inserted element.
         */
        template <typename InsertIterator>
        iterator insert(InsertIterator first, const InsertIterator last)
        {
            iterator it = base.begin();
            for(; first != last; ++first)
                it = insert(it, *first);
            return it;
        }

        void clear() { base.clear(); }
        void pop_back() { base.pop_back(); }
        void pop_front() { base.pop_front(); }

        T& back() { return base.back(); }
        T& front() { return base.front(); }
        const T& back() const { return base.back(); }
        const T& front() const { return base.front(); }

        size_t size() { return base.size(); }

        explicit operator std::list<T, Alloc>()
        {
            return base;
        };

        explicit operator std::vector<T, Alloc>()
        {
            std::vector<T, Alloc> target;
            target.reserve(base.size());
            target.insert(target.end(), base.begin(), base.end());
            return target;
        };

        iterator begin() { return base.begin(); }
        iterator end() { return base.end(); }
        const_iterator begin() const { return base.begin(); }
        const_iterator end() const { return base.end(); }
        const_iterator cbegin() const { return base.cbegin(); }
        const_iterator cend() const { return base.cend(); }
        reverse_iterator rbegin() { return base.rbegin(); }
        reverse_iterator rend() { return base.rend(); }
        const_reverse_iterator rbegin() const { return base.rbegin(); }
        const_reverse_iterator rend() const { return base.rend(); }
        const_reverse_iterator crbegin() const { return base.crbegin(); }
        const_reverse_iterator crend() const { return base.crend(); }
    };
}

#endif //MDLUTILS_SORTED_LIST_HPP
