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
    // const char *__cdecl mdl::type_name<...>(void)
#define PF_PREFIX "const char *__cdecl mdl::type_name<"
#define PF_SUFFIX ">(void)"
#else
    // const char* mdl::type_name() [T = ...]
#define PF_PREFIX "const char* mdl::type_name() [T = "
#define PF_SUFFIX "]"
#endif

    /* Check the compile-time type name passed as the function parameter
     * @T the type for which the typename should be determined.
     *
     * @return the typename T as understood by compiler's macro __PRETTY_FUNCTION__ (clang, gcc) or __FUNCSIG__ (msvc).
     */
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
        return pf;
    }

    /* Check the compile-time type name passed as the function parameter
     * @T the type for which the typename should be determined.
     *
     * @return the typename T as understood by compiler's macro __PRETTY_FUNCTION__ (clang, gcc) or __FUNCSIG__ (msvc).
     */
    template<typename T>
    inline std::string type_name_s()
    {
        return type_name<T>();
    }

    /* Check whether the type T is a specialization of template Template
     * @Template Base template class
     * @T specialized typename
     */
    template<template<typename...> class Template, typename T>
    struct is_specialization_of : std::false_type
    {
    };

    /// @inherit
    template<template<typename...> class Template, typename... Args>
    struct is_specialization_of<Template, Template<Args...> > : std::true_type
    {
    };


}

#endif //MDLUTILS_TYPEINFO_HPP
