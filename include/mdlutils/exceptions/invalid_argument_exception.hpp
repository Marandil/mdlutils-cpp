//
// Created by marandil on 17.08.15.
//

#ifndef MDLUTILS_EXCEPTIONS_INVALID_ARGUMENT_EXCEPTION_HPP
#define MDLUTILS_EXCEPTIONS_INVALID_ARGUMENT_EXCEPTION_HPP

#include <string>
#include <functional>

#include <mdlutils/string_utils.hpp>

#include <mdlutils/typedefs.hpp>
#include <mdlutils/exceptions/base_exception.hpp>

namespace mdl
{
    template<typename T>
    class invalid_argument_exception : public base_exception
    {
    protected:
        T value;
    public:
        template<typename Converter=std::function<std::string(const T &)>>
        invalid_argument_exception(const std::string &file, int line, const std::string &function,
                                   const std::string &customErrorMessage, const std::string &argName,
                                   const T &value, const Converter &converter = stringify<T>) :
                base_exception(file, line, function,
                               customErrorMessage + " (argument " + argName + " with value " + converter(value) + ")"),
                value(value) { commit_message(); }

        template<typename Converter=std::function<std::string(const T &)>>
        invalid_argument_exception(const std::string &file, int line, const std::string &function,
                                   const std::string &argName,
                                   const T &value, const Converter &converter = stringify<T>) :
                base_exception(file, line, function,
                               "Argument " + argName + " with value " + converter(value)),
                value(value) { commit_message(); }
    protected:
        /// @inherit
        virtual const std::string &tag() const
        {
            const static std::string tag = "InvalidArgumentException";
            return tag;
        }
    };
}

#endif //MDLUTILS_EXCEPTIONS_INVALID_ARGUMENT_EXCEPTION_HPP
