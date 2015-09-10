//
// Created by marandil on 08.09.15.
//

#ifndef MDLUTILS_TYPES_CONST_VECTOR_HPP
#define MDLUTILS_TYPES_CONST_VECTOR_HPP

#include <new>
#include <memory>

namespace mdl
{
    template<typename T, typename Alloc=std::allocator<T>>
    class const_vector
    {
    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename std::allocator_traits<allocator_type>::pointer pointer;
        typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;

        // A random access iterator (pointer)
        typedef pointer iterator;
        // A const random access iterator (const_pointer)
        typedef const_pointer const_iterator;
        // A signed integral type representing the differences between the iterators.
        typedef typename std::iterator_traits<iterator>::difference_type difference_type;
        // An unsigned integral type representing the size of the container.
        typedef size_t size_type;


    protected:
        size_t array_size;
        const pointer p_begin, p_end;

        Alloc alloc;

        const_vector(size_t size, std::nullptr_t null) : array_size(size), alloc(), p_begin(alloc.allocate(size)),
                                                         p_end(p_begin + array_size) { }

    public:
        const_vector(size_t size, Alloc alloc = Alloc()) :
                array_size(size),
                alloc(alloc),
                p_begin(alloc.allocate(size)),
                p_end(p_begin + array_size)
        {
            while (size--)
                alloc.construct(&p_begin[size]);
        }

        template<typename... Args>
        const_vector(size_t size, Args... args) : array_size(size), alloc(), p_begin(alloc.allocate(size)),
                                                  p_end(p_begin + array_size)
        {
            while (size--)
                alloc.construct(&p_begin[size], args...);
        }

        const_vector(const const_vector<T, Alloc> &other) = delete;

        const_vector(const_vector<T, Alloc> &&other) : array_size(other.array_size), alloc(other.alloc),
                                                       p_begin(other.p_begin), p_end(other.p_end) { }

        ~const_vector(void)
        {
            for (unsigned i = array_size; i--;)
                alloc.destroy(&p_begin[i]);
            alloc.deallocate(p_begin, array_size);
            return;
        }

        iterator begin() { return p_begin; }

        const_iterator begin() const { return p_begin; }

        iterator end() { return p_end; }

        const_iterator end() const { return p_end; }

        const_iterator cbegin() const { return p_begin; }

        const_iterator cend() const { return p_end; }

        size_type size() { return array_size; }

        size_type max_size() { return array_size; }

        size_type capacity() { return array_size; }

        bool empty() { return !array_size; }

        reference operator[](size_t index) { return p_begin[index]; }

        const_reference operator[](size_t index) const { return p_begin[index]; }

        reference at(size_t index) { return p_begin[index]; }

        const_reference at(size_t index) const { return p_begin[index]; }

        reference front(size_t index) { return p_begin[0]; }

        const_reference front(size_t index) const { return p_begin[0]; }

        reference back(size_t index) { return p_end[-1]; }

        const_reference back(size_t index) const { return p_end[-1]; }

        pointer data() { return p_begin; }

        const_pointer data() const { return p_begin; }

        allocator_type get_allocator() const { return alloc; }

        template<typename... Args>
        static const_vector<T, Alloc> make_indexed(size_t size, Args... args)
        {
            const_vector<T, Alloc> result(size, nullptr);
            while (size--)
                result.alloc.construct(&(result.p_begin[size]), size, args...);
            return result;
        };


    };
}

#endif //MDLUTILS_TYPES_CONST_VECTOR_HPP
