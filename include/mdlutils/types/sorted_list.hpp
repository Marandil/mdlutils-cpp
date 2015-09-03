//
// Created by marandil on 28.08.15.
//

#ifndef MDLUTILS_SORTED_LIST_HPP
#define MDLUTILS_SORTED_LIST_HPP

#include <list>
#include <functional>

namespace mdl
{
    template<typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    class sorted_list
    {
    protected:
        typedef std::list<T, Alloc> base_type;
        base_type base;
        Compare comp;
        Alloc alloc;

    public:
        typedef Compare key_compare;
        typedef Compare value_compare;

        typedef T key_type;
        typedef T value_type;

        typedef Alloc allocator_type;

        typedef T &reference;
        typedef const T &const_reference;

        typedef typename std::allocator_traits<allocator_type>::pointer pointer;
        typedef typename std::allocator_traits<allocator_type>::pointer const_pointer;

        // const iterator to prevent modifying already inserted values.
        typedef typename base_type::const_iterator iterator;
        typedef typename base_type::const_iterator const_iterator;
        // const iterator to prevent modifying already inserted values.
        typedef typename base_type::const_reverse_iterator reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;

        typedef typename std::iterator_traits<iterator>::difference_type difference_type;
        typedef size_t size_type;


    protected:
        /* Check whether the *ai >= b and b >= *ai in terms of comp being < operator
         * @ai iterator to an element of base list.
         * @b an element of type T.
         * @return true, if *ai is not smaller than b, nor b is smaller than *ai.
         */
        bool equiv(iterator ai, const T &b) const
        {
            return (!(comp(*ai, b) || comp(b, *ai)));
        }

        /* Check whether the *ai >= *bi and *bi >= *ai in terms of comp being < operator
         * @ai iterator to an element of base list.
         * @bi iterator to an element of base list.
         * @return true, if *ai is not smaller than *bi, nor *bi is smaller than *ai.
         */
        bool equiv(iterator ai, iterator bi)
        {
            return (!(comp(*ai, *bi) || comp(*bi, *ai)));
        }

    public:

        /* Default, empty constructor.
         * @comp Comparator object.
         * @alloc Allocator object.
         */
        sorted_list(const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type()) :
                base(alloc), comp(comp), alloc(alloc) { }

        /* Initializer list constructor
         * @il An initializer_list automatically constructed from initializer list declarators.
         * @comp Comparator object.
         * @alloc Allocator object.
         */
        sorted_list(std::initializer_list<value_type> il,
                    const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type()) :
                base(alloc), comp(comp), alloc(alloc)
        {
            iterator it = begin();
            for (auto a : il)
                it = insert(it, std::move(a));
        }

        /* Constructs a container with elements coming from the range [first,last)
         * @InputIterator Iterator type;
         * @first iterator specifying the first element in the inserting range.
         * @last iterator specifying the element after the last in the inserting range.
         * @comp Comparator object.
         * @alloc Allocator object.
         */
        template<typename InputIterator>
        sorted_list(InputIterator first, InputIterator last,
                    const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type()) : base(comp, alloc), comp(comp), alloc(alloc)
        {
            iterator it = begin();
            for (; first != last; ++first)
                it = insert(it, *first);
        }

        // Copy constructor
        sorted_list(const sorted_list<T, Compare, Alloc> &x) :
                base(x.base),
                comp(x.comp),
                alloc(x.alloc) { }

        // Move constructor
        sorted_list(sorted_list<T, Compare, Alloc> &&x) :
                base(std::move(x.base)),
                comp(std::move(x.comp)),
                alloc(std::move(x.alloc)) { }

        /* Create and insert a new element.
         * @args the values to be passed to the constructor.
         * @return an iterator that points to the inserted element.
         */
        template<typename... Args>
        iterator emplace(Args &&... args)
        {
            return emplace_hint(base.begin(), args...);
        }

        /* Create and insert a new element,  given an hinted position.
         * @position hinted position to consider while inserting.
         * @args the values to be passed to the constructor.
         * @return an iterator that points to the inserted element.
         */
        template<typename... Args>
        iterator emplace_hint(iterator position, Args &&... args)
        {
            T object(args...);

            iterator begin = base.begin(), end = base.end(), it = position;

            if (begin == end) // if the container is empty
                return base.insert(begin, std::move(object));

            if (comp(base.back(), object)) // shortcut, if the element is greater than the last element in the list
                return base.insert(end, std::move(object));
            if (comp(object, base.front())) // shortcut, if the element is smaller than the first element in the list
                return base.insert(begin, std::move(object));

            if (comp(object, *it)) // the value is smaller than the hinted position, therefore insert it to the left:
            {
                for (; it != begin; --it)
                    if (comp(*it, object)) break;
                return base.insert(++it,
                                   std::move(object)); // we need to insert at an element greater than the smaller one
            }
            else // the value is greater-or-equal than the hinted position, therefore insert it to the right:
            {
                for (; it != end; ++it)
                    if (comp(object, *it)) break;
                return base.insert(it, std::move(object));
            }
        }

        /* Insert element
         * @value the value to be inserted.
         * @return an iterator that points to the inserted element.
         */
        iterator insert(const T &value)
        {
            iterator it = base.begin(), end = base.end();

            if (it == end) // if the container is empty
                return base.insert(it, value);

            if (comp(base.back(), value)) // shortcut, if the element is greater than the last element in the list
                return base.insert(end, value);

            for (; it != end; ++it)
                if (comp(value, *it)) break;
            return base.insert(it, value);
        }

        /* Insert element, given an hinted position.
         * @position hinted position to consider while inserting.
         * @value the value to be inserted.
         * @return an iterator that points to the inserted element.
         */
        iterator insert(iterator position, const T &value)
        {
            iterator begin = base.begin(), end = base.end(), it = position;

            if (begin == end) // if the container is empty
                return base.insert(begin, value);

            if (comp(base.back(), value)) // shortcut, if the element is greater than the last element in the list
                return base.insert(end, value);
            if (comp(value, base.front())) // shortcut, if the element is smaller than the first element in the list
                return base.insert(begin, value);

            if (comp(value, *it)) // the value is smaller than the hinted position, therefore insert it to the left:
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
         * @first iterator specifying the first element in the inserting range
         * @last iterator specifying the element after the last in the inserting range
         * @return an iterator that points to the last inserted element.
         */
        template<typename InsertIterator>
        iterator insert(InsertIterator first, const InsertIterator last)
        {
            return insert(base.begin(), first, last);
        }


        /* Insert range of elements, given a hinted position
         * @position hinted position to consider while inserting.
         * @first iterator specifying the first element in the inserting range.
         * @last iterator specifying the element after the last in the inserting range.
         * @return an iterator that points to the last inserted element.
         */
        template<typename InsertIterator>
        iterator insert(iterator position, InsertIterator first, const InsertIterator last)
        {
            iterator it = position;
            for (; first != last; ++first)
                it = insert(it, *first);
            return it;
        }

        /* Searches the list for the first element equivalent to <value>
         * @value key of the element to search for.
         * @return valid iterator to the first element found, or <end()> if the element was not found.
         */
        iterator find(const T &value) const
        {
            iterator begin = base.begin(), end = base.end(), it;

            // To improve stability, just go from the beginning to the end:
            for (it = begin; it != end && comp(*it, value); ++it); // go up, until you meet a value >= or end
            if (it != end && !comp(value, *it)) return it; // if the element is <= (and not end) return it
            return end; // otherwise not found
        }

        /* Searches the container for elements equivalent to value and returns the number of matches.
         * @value key of the element to search for.
         * @return number of elements in the container that are equivalent to <value>.
         */
        size_t count(const T &value) const
        {
            iterator begin = find(value), end = base.end();
            size_t c = 0;
            for (; begin != end && equiv(begin, value); ++c, ++begin); // while not end and in the equivalence class
            return c;
        }

        /* Returns the iterator for the first element equivalent to value (equivalent to <find>)
         * @value key of the element to search for.
         * @return valid iterator to the first element found, or end() if the element was not found.
         */
        iterator lower_bound(const T &value) const { return find(value); }

        /* Returns the iterator for the first element after <lower_bound> not equivalent to value
         * @value key of the element to search for.
         * @return valid iterator to the first element found after the equivalence class of value.
         */
        iterator upper_bound(const T &value) const
        {
            iterator begin = find(value), end = base.end();
            for (; begin != end && equiv(begin, value); ++begin); // while not end and in the equivalence class
            return begin;
        }

        /* Returns the bounds of a range that includes all the elements in the container that are equivalent to <value>.
         * @value key of the element to search for.
         * @return a pair, whose member pair::first is the lower bound of the range (the same as lower_bound), and pair::second is the upper bound (the same as upper_bound).
         */
        std::pair<iterator, iterator> equal_range(const T &value) const
        {
            iterator begin = find(value), end = base.end(), it = begin;
            for (; it != end && equiv(it, value); ++it); // while not end and in the equivalence class
            return std::make_pair(begin, it);
        }

        /* Erase element given a hinted position (not necessarily containing the value)
         * @position hinted position to consider while removing.
         * @value value to be removed from the set.
         * @return the number of elements removed.
         */
        size_t erase(iterator position, const T &value)
        {
            if (base.empty()) // if the container is empty
                return 0;
            if (comp(base.back(), value)) // shortcut, if the element is greater than the last element in the list
                return 0;
            if (comp(value, base.front())) // shortcut, if the element is smaller than the first element in the list
                return 0;

            size_t removed = 0;
            while (position != base.end() && equiv(position, value)) // '!( < || > )' <==> '==', shortcut if hinted == value
            {
                position = base.erase(position);
                ++removed;
            }

            iterator begin = base.begin(), end = base.end(), it;

            // To improve stability, just go from the it to the end, and/or from the beginning to the it:
            for (it = position; it != end && comp(*it, value); ++it); // go up, until you meet a value >= or end
            while (it != end && equiv(it, value)) // while not end and in the equivalence class
            {
                it = base.erase(it);
                ++removed;
            }
            for (it = begin; it != position && comp(*it, value); ++it); // go up, until you meet a value >= or position
            while (it != position && equiv(it, value)) // while not end and in the equivalence class
            {
                it = base.erase(it);
                ++removed;
            }

            return removed;
        }

        /* Erase element (by value)
         *
         * @value value to be removed from the set.
         * @return the number of elements removed.
         */
        size_t erase(const T &value)
        {
            return erase(base.begin(), value);
        }

        /* Erase element
         *
         * @position iterator pointing to a single element to be removed.
         * @return the iterator to the item following the erased element.
         */
        iterator erase(iterator position) { return base.erase(position); }

        /* Erase elements
         *
         * @first iterator pointing to the first element to be removed.
         * @last iterator specifying the element after the last element to be removed.
         * @return the iterator to the item following the last erased element.
         */
        iterator erase(iterator first, iterator last) { return base.erase(first, last); }

        /* Clears the underlying list container */
        void clear() { base.clear(); }

        /* Erases the last element from the list */
        void pop_back() { base.pop_back(); }

        /* Erases the first element from the list */
        void pop_front() { base.pop_front(); }

        /* Returns the last element of the list
         *
         * @return The last element of the list
         */
        const T &back() const { return base.back(); }

        /* Returns the first element of the list
         *
         * @return The first element of the list
         */
        const T &front() const { return base.front(); }

        /* Returns the number of elements in the list
         *
         * @return The number of elements in the list
         */
        size_t size() const { return base.size(); }

        /* Checks, whether the underlying list container is empty
         *
         * @return true if the list is empty, false otherwise
         */
        bool empty() const { return base.empty(); }

        /* Explicit conversion operator to std::list<T>
         *
         * Returns the copy of the underlying list container
         */
        explicit operator std::list<T, Alloc>()
        {
            return base;
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

        /* Returns a copy of the comparison object used by the container. */
        Compare key_comp() const { return comp; }

        /* Returns a copy of the comparison object used by the container. */
        Compare value_comp() const { return comp; }

        /* Returns a copy of the allocator object used by the container. */
        Alloc get_allocator() const { return alloc; }

        /* Returns the maximum number of elements that the container can hold */
        size_t max_size() const { return base.max_size(); }
    };

}

// multiset comparison operators, require inclusion of set
#include <set>

template <class T, class Compare, class Allocator>
bool operator== ( const mdl::sorted_list<T,Compare,Allocator>& lhs,
                  const std::multiset<T,Compare,Allocator>& rhs )
{
    Compare comp = lhs.key_comp();
    auto itl = lhs.begin(), lhs_end = lhs.end();
    auto itr = rhs.begin(), rhs_end = rhs.end();
    if(rhs.size() != lhs.size()) return false;
    for(; itl != lhs_end; ++itl, ++itr)
        if(comp(*itl, *itr) || comp(*itr, *itl))
            return false;
    return true;
};

template <class T, class Compare, class Allocator>
bool operator!= ( const mdl::sorted_list<T,Compare,Allocator>& lhs,
                  const std::multiset<T,Compare,Allocator>& rhs )
{
    return !(lhs == rhs);
};

template <class T, class Compare, class Allocator>
bool operator== ( const std::multiset<T,Compare,Allocator>& lhs,
                  const mdl::sorted_list<T,Compare,Allocator>& rhs )
{
    return rhs == lhs;
};

template <class T, class Compare, class Allocator>
bool operator!= ( const std::multiset<T,Compare,Allocator>& lhs,
                  const mdl::sorted_list<T,Compare,Allocator>& rhs )
{
    return !(rhs == lhs);
};

#endif //MDLUTILS_SORTED_LIST_HPP
