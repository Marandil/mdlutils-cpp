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
        /* Construct an invalid_argument_exception with a custom error message.
         * @file Name of the file file in which exception has occurred.
         * @line Line at which exception has occurred.
         * @function The signature of the function that throws the exception.
         * @customErrorMessage User-defined message for the exception message.
         * @argName Name of the invalid argument (variable).
         * @value Invalid argument value.
         * @converter to-string converter used to get textual representation of <value>
         */
        template<typename Converter=std::function<std::string(const T &)>>
        invalid_argument_exception(const std::string &file, int line, const std::string &function,
                                   const std::string &customErrorMessage, const std::string &argName,
                                   const T &value, const Converter &converter = stringify<T>) :
                base_exception(file, line, function,
                               customErrorMessage + " (argument " + argName + " with value " + converter(value) + ")"),
                value(value) { }

        /* Construct an invalid_argument_exception without a custom error message.
         * @file Name of the file file in which exception has occurred.
         * @line Line at which exception has occurred.
         * @function The signature of the function that throws the exception.
         * @argName Name of the invalid argument (variable).
         * @value Invalid argument value.
         * @converter to-string converter used to get textual representation of <value>
         */
        template<typename Converter=std::function<std::string(const T &)>>
        invalid_argument_exception(const std::string &file, int line, const std::string &function,
                                   const std::string &argName,
                                   const T &value, const Converter &converter = stringify<T>) :
                base_exception(file, line, function,
                               "Argument " + argName + " with value " + converter(value)),
                value(value) { }

        // TODO: Change the name (maybe)

        /* Return the value with which the exception has been thrown
         *
         * @return The original value of type T (as a const reference to avoid copying)
         */
        const T& throw_value() { return value; }

    protected:
        /// @inherit
        virtual const std::string &tag() const
        {
            const static std::string tag = "InvalidArgumentException";
            return tag;
        }
    };

    /* Construct an invalid_argument_exception for an unspecified type - automatic type resolution provided.
     * @file Name of the file file in which exception has occurred.
     * @line Line at which exception has occurred.
     * @function The signature of the function that throws the exception.
     * @argName Name of the invalid argument (variable).
     * @value Invalid argument value.
     * @converter to-string converter used to get textual representation of <value>
     *
     * @return An instance of <invalid_argument_exception> with auto-determined T, and given arguments.
     */
    template<typename T, typename Converter=std::function<std::string(const T &)>>
    invalid_argument_exception<T> make_ia_exception(const std::string &file, int line, const std::string &function,
                                                    const std::string &argName,
                                                    const T &value, const Converter &converter = stringify<T>)
    {
        return invalid_argument_exception<T>(file, line, function, argName, value, converter);
    };

    /* Construct an invalid_argument_exception for an unspecified type - automatic type resolution provided.
     * @file Name of the file file in which exception has occurred.
     * @line Line at which exception has occurred.
     * @function The signature of the function that throws the exception.
     * @customErrorMessage User-defined message for the exception message.
     * @argName Name of the invalid argument (variable).
     * @value Invalid argument value.
     * @converter to-string converter used to get textual representation of <value>
     *
     * @return An instance of <invalid_argument_exception> with auto-determined T, and given arguments.
     */
    template<typename T, typename Converter=std::function<std::string(const T &)>>
    invalid_argument_exception<T> make_ia_exception(const std::string &file, int line, const std::string &function,
                                                    const std::string &customErrorMessage, const std::string &argName,
                                                    const T &value, const Converter &converter = stringify<T>)
    {
        return invalid_argument_exception<T>(file, line, function, customErrorMessage, argName, value, converter);
    };
}

#endif //MDLUTILS_EXCEPTIONS_INVALID_ARGUMENT_EXCEPTION_HPP
