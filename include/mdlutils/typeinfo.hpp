//
// Created by marandil on 04.09.15.
//

#ifndef MDLUTILS_TYPEINFO_HPP
#define MDLUTILS_TYPEINFO_HPP

#ifdef _MSC_VER
    #define __NICE_FUNCTION__ __FUNCSIG__
#else
    #define __NICE_FUNCTION__ __PRETTY_FUNCTION__
#endif


#include <string>
#include <cstdint>

#include <cstring>

namespace mdl
{
    // TODO: Check the compatibility with MSVC

    #ifdef _MSC_VER
        #define PF_PREFIX "const char* mdl::type_name<"
        #define PF_SUFFIX ">()"
    #else
        #define PF_PREFIX "const char* mdl::type_name() [T = "
        #define PF_SUFFIX "]"
    #endif

    template<typename T>
    inline const char *type_name()
    {
        constexpr size_t pfsize = sizeof(__NICE_FUNCTION__);
        constexpr size_t prefix_offs = sizeof(PF_PREFIX) - 1;
        constexpr size_t suffix_offs = sizeof(PF_SUFFIX) - 1;
        constexpr size_t typename_size = pfsize - prefix_offs - suffix_offs;
        static char pf[typename_size];
        static bool f = false;
        if (!f)
        {
            memcpy(pf, __NICE_FUNCTION__ + prefix_offs, typename_size);
            pf[typename_size - 1] = '\0';
            f = true;
        }
        std::cout << __NICE_FUNCTION__ << std::endl;
        std::cout << pf << std::endl;
        return pf;
    }

    template <typename T>
    inline std::string type_name_s()
    {
        return type_name<T>();
    }
}

#endif //MDLUTILS_TYPEINFO_HPP
