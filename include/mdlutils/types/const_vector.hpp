//
// Created by marandil on 08.09.15.
//

#ifndef MDLUTILS_TYPES_CONST_VECTOR_HPP
#define MDLUTILS_TYPES_CONST_VECTOR_HPP

#include <new>
#include <memory>

namespace mdl
{
    template <typename T, typename Alloc=std::allocator<T>>
    class const_vector
    {
    protected:
        size_t size;
        T * memory;

        enum class alloc_type
        {
            ALLOC_NEW_ARRAY,
            ALLOC_ALLOC
        };

        Alloc alloc;
        alloc_type allocated_with;

        const_vector(size_t size, std::nullptr_t null) : size(size), alloc(), memory(alloc.allocate(size)), allocated_with(alloc_type::ALLOC_ALLOC) { }
    public:
        const_vector(size_t size, Alloc alloc =Alloc()) : size(size), memory(new T[size]), alloc(alloc), allocated_with(alloc_type::ALLOC_NEW_ARRAY) { }

        template <typename... Args>
        const_vector(size_t size, Args... args) : size(size), alloc(), memory(alloc.allocate(size)), allocated_with(alloc_type::ALLOC_ALLOC)
        {
            while(size--)
                alloc.construct(&memory[size], args...);
        }

        const_vector(const const_vector<T, Alloc>& other) = delete;
        const_vector(const_vector<T, Alloc>&& other) : size(other.size), memory(other.memory), alloc(other.alloc), allocated_with(other.allocated_with) { }

        ~const_vector(void)
        {
            switch (allocated_with)
            {
                case alloc_type::ALLOC_NEW_ARRAY:
                    delete[] memory;
                    return;
                case alloc_type::ALLOC_ALLOC:
                    for(unsigned i = size; i--;)
                        alloc.destroy(&memory[i]);
                    alloc.deallocate(memory, size);
                    return;
            }
        }

        T&operator [](size_t index)
        {
            return memory[index];
        }

        const T&operator [](size_t index) const
        {
            return memory[index];
        }

        template <typename... Args>
        static const_vector<T, Alloc> make_indexed(size_t size, Args... args)
        {
            const_vector<T, Alloc> result(size, nullptr);
            while(size--)
                result.alloc.construct(&(result.memory[size]), size, args...);
            return result;
        };
    };
}

#endif //MDLUTILS_TYPES_CONST_VECTOR_HPP
