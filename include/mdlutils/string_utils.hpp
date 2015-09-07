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
    };

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
            return "Pointer [" + mdl::type_name_s<T>() + "] @ 0x" + hexify(*((size_t *) (value)));
        }

        template<typename T>
        std::string stringify_helper(typename std::enable_if<std::is_function<T>::value, const T &>::type value)
        {
            return "Function [" + mdl::type_name_s<T>() + "] @ 0x" + hexify((size_t) ((void *) (&value)));
        }

        template<typename T>
        std::string stringify_helper(const T &value)
        {
            return "Element [" + mdl::type_name_s<T>() + "] @ 0x" + hexify((size_t) ((void *) (&value))) +
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
}

#endif //MDLUTILS_STRING_UTILS_HPP
