//
// Created by marandil on 03.09.15.
//

#ifndef MDLUTILS_STRING_UTILS_HPP
#define MDLUTILS_STRING_UTILS_HPP

#include <string>
#include <cstdint>

namespace mdl
{
    template<typename T>
    inline std::string stringify(const T &value) { return "Element @ 0x" + std::to_string((size_t)((void*)(&value))) + " of size (at least) " + std::to_string(sizeof(T)); }

    template<>
    inline std::string stringify<std::string>(const std::string &value) { return value; }

    template<>
    inline std::string stringify<char *>(char *const &value) { return std::string(value); }

    template<>
    inline std::string stringify<int8_t>(const int8_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<int16_t>(const int16_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<int32_t>(const int32_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<int64_t>(const int64_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<uint8_t>(const uint8_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<uint16_t>(const uint16_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<uint32_t>(const uint32_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<uint64_t>(const uint64_t &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<float>(const float &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<double>(const double &value) { return std::to_string(value); }

    template<>
    inline std::string stringify<long double>(const long double &value) { return std::to_string(value); }
}

#endif //MDLUTILS_STRING_UTILS_HPP
