//
// Created by marandil on 17.08.15.
//

#ifndef MDLUTILS_EXCEPTIONS_INVALID_ARGUMENT_EXCEPTION_HPP
#define MDLUTILS_EXCEPTIONS_INVALID_ARGUMENT_EXCEPTION_HPP

#include <string>

#include <mdlutils/typedefs.hpp>
#include <mdlutils/exceptions/base_exception.hpp>

namespace mdl
{
    class invalid_argument_exception : public base_exception
    {
    public:
        invalid_argument_exception(const std::string &customErrorMessage, const std::string &argName,
                                   const std::string &printableValue, const std::string &file, int line) :
                base_exception(customErrorMessage + " (argument " + argName + " with value " + printableValue + ")",
                               file, line) { }

        invalid_argument_exception(const std::string &argName, const std::string &printableValue,
                                   const std::string &file, int line) :
                base_exception("Argument " + argName + " with value " + printableValue, file, line) { }

    protected:
        /// @inherit
        virtual const std::string &getTag() const
        {
            const static std::string tag = "InvalidArgumentException";
            return tag;
        }
    };
}

#endif //MDLUTILS_EXCEPTIONS_INVALID_ARGUMENT_EXCEPTION_HPP
