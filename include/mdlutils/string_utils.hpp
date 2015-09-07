//
// Created by marandil on 03.09.15.
//

#ifndef MDLUTILS_STRING_UTILS_HPP
#define MDLUTILS_STRING_UTILS_HPP

#include <string>
#include <cstdint>

#include <cstring>

#include <mdlutils/typeinfo.hpp>

namespace mdl
{
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
    inline std::string stringify(const T &value)
    {
        return "Element [" + mdl::type_name_s<T>() + "] @ 0x" + std::to_string((size_t)((void*)(&value))) + " of size (at least) " + std::to_string(sizeof(T));
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

    /* Instantiation of <stringify> for 8-bit signed integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<int8_t>(const int8_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for 16-bit signed integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<int16_t>(const int16_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for 32-bit signed integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<int32_t>(const int32_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for 64-bit signed integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<int64_t>(const int64_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for 8-bit unsigned integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<uint8_t>(const uint8_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for 16-bit unsigned integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<uint16_t>(const uint16_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for 32-bit unsigned integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<uint32_t>(const uint32_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for 64-bit unsigned integer.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<uint64_t>(const uint64_t &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for single floating point number.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<float>(const float &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for double floating point number.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<double>(const double &value) { return std::to_string(value); }

    /* Instantiation of <stringify> for long double floating point number.
     * @value value to convert to string.
     *
     * @return textual representation of <value>.
     */
    template<>
    inline std::string stringify<long double>(const long double &value) { return std::to_string(value); }
}

#endif //MDLUTILS_STRING_UTILS_HPP
