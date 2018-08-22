//
// Created by marandil on 11.09.15.
//

#ifndef MDLUTILS_EXCEPTIONS_INVALID_STATE_EXCEPTION_HPP
#define MDLUTILS_EXCEPTIONS_INVALID_STATE_EXCEPTION_HPP

#include <mdlutils/exceptions/base_exception.hpp>
#include <mdlutils/exceptions/invalid_argument_exception.hpp> // this exception is based upon the IAE

namespace mdl
{
    
    template<typename T>
    class invalid_state_exception : public base_exception
    {
    protected:
        const T &object;
    public:
        /* Construct an invalid_state_exception with a custom error message.
         * @file Name of the file file in which exception has occurred.
         * @line Line at which exception has occurred.
         * @function The signature of the function that throws the exception.
         * @customErrorMessage User-defined message for the exception message.
         * @object Invalid object reference.
         * @converter to-string converter used to get textual representation of <value>
         */
        template<typename Converter = std::string(&)(const T &),
                class = is_converter_t<Converter, T>>
        invalid_state_exception(const std::string &file, int line, const std::string &function,
                                const std::string &customErrorMessage,
                                const T &object, Converter converter = stringify<T>) :
                base_exception(file, line, function,
                               customErrorMessage + " (object of type " + type_name_s<T>() + " with value " +
                               converter(object) + ")"),
                object(object) {}
        
        /* Construct an invalid_state_exception without a custom error message.
         * @file Name of the file file in which exception has occurred.
         * @line Line at which exception has occurred.
         * @function The signature of the function that throws the exception.
         * @object Invalid object reference.
         * @converter to-string converter used to get textual representation of <value>
         */
        template<typename Converter = std::string(&)(const T &),
                class = is_converter_t<Converter, T>>
        invalid_state_exception(const std::string &file, int line, const std::string &function,
                                const T &object, Converter converter = stringify<T>) :
                base_exception(file, line, function,
                               "Object of type " + type_name_s<T>() + " with value " + converter(object)),
                object(object) {}
        
        /* Return the reference to the object with which the exception has been thrown.
         *
         * Warning, the reference might be invalid by the time the exception is handled, so any later use of the variable may cause undefined behaviour.
         *
         * @return The original object of type T (as a const reference)
         */
        const T &throw_object() { return object; }
    
    protected:
        /// @inherit
        virtual const std::string &tag() const
        {
            const static std::string tag = "InvalidStateException";
            return tag;
        }
    };
    
}

#endif //MDLUTILS_EXCEPTIONS_INVALID_STATE_EXCEPTION_HPP
