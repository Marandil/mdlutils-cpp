//
// Created by marandil on 03.09.15.
//

#ifndef MDLUTILS_STRING_UTILS_HPP
#define MDLUTILS_STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include <cstring>
#include <functional>

#include <mdlutils/typeinfo.hpp>

namespace mdl
{
    /* Convert any integral type to it's hexadecimal representation
     * @T An integral type
     * @value Value to be converted
     *
     * @return std::string containing (aligned) hexadecimal representation of value
     */
    template <typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    std::string hexify(T value)
    {
        std::stringstream buffer;
        buffer << std::hex << std::setfill('0') << std::setw(sizeof(T) * 2) << value;
        return buffer.str();
    }
    template <> inline
    std::string hexify(unsigned char value)
    {
        std::stringstream buffer;
        buffer << std::hex << std::setfill('0') << std::setw(2) << (uint16_t)(value);
        return buffer.str();
    };
    template <> inline
    std::string hexify(char value)
    {
        return hexify(reinterpret_cast<unsigned char&>(value));
    };
    
    /* Convert an array of integral type to it's hexadecimal representation
     * @T An integral type
     * @value Value to be converted
     *
     * @return std::string containing (aligned) hexadecimal representation of value
     */
    template <typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    std::string hexify(const T* value, size_t count)
    {
        std::stringstream buffer;
        for(const T* end = value + count; value != end; ++value)
            buffer << std::hex << std::setfill('0') << std::setw(sizeof(T) * 2) << (*value);
        return buffer.str();
    }
    template <> inline
    std::string hexify(const unsigned char* value, size_t count)
    {
        std::stringstream buffer;
        for(const unsigned char* end = value + count; value != end; ++value)
            buffer << std::hex << std::setfill('0') << std::setw(2) << (uint16_t)(*value);
        return buffer.str();
    }
    template <> inline
    std::string hexify(const char* value, size_t count)
    {
        return hexify(reinterpret_cast<const unsigned char*>(value), count);
    }


    // Forward declaration
    template<typename T>
    std::string stringify (const T& value);

    namespace helper
    {
        template<typename T>
        std::string stringify_helper(typename std::enable_if<std::is_arithmetic<T>::value, const T &>::type value)
        {
            return std::to_string(value);
        }

        template<typename T>
        std::string stringify_helper(typename std::enable_if<std::is_pointer<T>::value, const T &>::type value)
        {
            return "Pointer [" + mdl::type_name_s<T>() + "] -> 0x" + hexify(reinterpret_cast<size_t>(value));
        }

        template<typename T>
        std::string stringify_helper(typename std::enable_if<std::is_function<T>::value, const T &>::type value)
        {
            return "Function [" + mdl::type_name_s<T>() + "] @ 0x" + hexify(reinterpret_cast<size_t>(&value));
        }

        template<typename V>
        std::string stringify_function(const std::function<V> &value)
        {
            return "Function Wrapper of [" + mdl::type_name_s<V>() + "]"; // TODO: Try to use target<...>()
        }

        template<typename T>
        std::string stringify_helper(typename std::enable_if<mdl::is_specialization_of<std::function, T>::value, const T &>::type value)
        {
            return stringify_function(value);
        }

        /* Instantiation of <stringify> for std::pair
         * @value std::pair to convert to string.
         *
         * @return stringify(value.first) + " and " + stringify(value.last)
         */
        template<typename T>
        std::string stringify_helper(typename std::enable_if<mdl::is_specialization_of<std::pair, T>::value, const T &>::type value)
        {
            return "std::pair of " + mdl::stringify(value.first) + " and " + mdl::stringify(value.second);
        }

        template <typename Tuple, size_t index>
        std::string stringify_tuple(typename std::enable_if<!index, const Tuple&>::type value)
        {
            return stringify(std::get<index>(value));
        }

        template <typename Tuple, size_t index>
        std::string stringify_tuple(typename std::enable_if<!!index, const Tuple&>::type value)
        {
            return stringify_tuple<Tuple, index - 1>(value) + ", " + stringify(std::get<index>(value));
        }

        /* Instantiation of <stringify> for std::tuple
         * @value std::pair to convert to string.
         *
         * @return stringify(value.first) + " and " + stringify(value.last)
         */
        template<typename T>
        std::string stringify_helper(typename std::enable_if<mdl::is_specialization_of<std::tuple, T>::value, const T &>::type value)
        {
            return "std::tuple of (" + stringify_tuple<T, std::tuple_size<T>::value - 1>(value) + ")";
        }

        template<typename T>
        std::string stringify_helper(const T &value)
        {
            return "Element [" + mdl::type_name_s<T>() + "] @ 0x" + hexify(reinterpret_cast<size_t>(&value)) +
                   " of size (at least) " + std::to_string(sizeof(T));
        }
    }

    /* Method providing a simple way of converting any type to a textual representation.
     * @value value to convert to string.
     *
     * Instantiate the template with a custom type, to provide a custom conversion method.
     * By default, strings and char*s are just re-created as a string object, numeric types are converted to string via
     * std::to_string method. The rest is represented as Element [(typename)] @ (address) of size (sizeof)
     *
     * @return textual representation of <value>
     */
    template<typename T>
    std::string stringify (const T& value)
    {
        return helper::stringify_helper<T>(value);
    }

    /* Instantiation of <stringify> for std::string
     * @value string to recreate.
     *
     * @return <value>
     */
    template<>
    inline std::string stringify<std::string>(const std::string &value) { return value; }

    /* Instantiation of <stringify> for char*
     * @value string to recreate
     *
     * @return std::string(<value>)
     */
    template<>
    inline std::string stringify<const char *>(const char *const &value) { return std::string(value); }

    template<typename Converter, typename T>
    using is_converter = typename std::enable_if<
            std::is_convertible<
                    typename std::result_of<Converter(const T&)>::type,
                    std::string>
            ::value>;

    template<typename Converter, typename T>
    using is_converter_t = typename is_converter<Converter, T>::type;
}

#endif //MDLUTILS_STRING_UTILS_HPP
