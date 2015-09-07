//
// Created by marandil on 17.08.15.
//

#ifndef MDLUTILS_EXCEPTIONS_BASE_EXCEPTION_HPP
#define MDLUTILS_EXCEPTIONS_BASE_EXCEPTION_HPP

#include <exception>
#include <memory>
#include <string>

#include <mdlutils/typedefs.hpp>
#include <mdlutils/typeinfo.hpp>

#ifndef mdl_throw
/* Custom exception throwing (adds filename, function header and exception line).
 * @ex exception class
 * @... exception class arguments.
 */
#define mdl_throw(ex, ...) throw ex(__FILE__, __LINE__, __NICE_FUNCTION__, __VA_ARGS__);
#endif

namespace mdl
{

    /* Virtual class for creating custom exceptions.
     */
    class base_exception : public std::exception
    {
    private:
        /* Message builder helper class for putting together pieces of what() messages */
        struct base_exception_message_builder
        {
            // Const reference to the base class
            const base_exception& base;

            /* Default constructor
             * @base const reference to the corresponding exception object
             */
            base_exception_message_builder(const base_exception& base) : base(base) { }

            // Message buffer
            std::string buffer{""};

            // Construct what() message, if the message buffer is empty, otherwise always return the same string.
            const std::string& message()
            {
                if(!buffer.size())
                    buffer = (base.tag() + ": " + base.message + "\n\tin function " + base.function + "\n\tat " + std::to_string(base.line) + " in " + base.file);
                return buffer;
            }
        };

        /* Unique pointer to the corresponding message_builder object.
         *
         * Each exception object has a corresponding message_builder object, that's created/deleted along with the exception.
         * The message_builder object lets the exception create (and store) the message string even if what() is called on a
         * const reference to an instance.
         */
        const std::unique_ptr<base_exception_message_builder> message_builder;
    public:
        /* Constructs exception instance.
         * @file Name of the file file in which exception has occurred.
         * @line Line at which exception has occurred.
         * @function The signature of the function that throws the exception.
         */
        base_exception(const std::string &file, int line, const std::string &function)
                : message(""), file(file), line(line), function(function), message_builder(new base_exception_message_builder(*this)) { }

        /* Constructs exception instance.
         * @err Exception message.
         * @file Name of the file file in which exception has occurred.
         * @line Line at which exception has occurred.
         * @function The signature of the function that throws the exception.
         */
        base_exception(const std::string &file, int line, const std::string &function, const std::string &err)
                : message(err), file(file), line(line), function(function), message_builder(new base_exception_message_builder(*this)) { }

        /* Copy constructor */
        base_exception(const base_exception& other) : message(other.message), file(other.file), line(other.line),
                                                      function(other.function), message_builder(new base_exception_message_builder(*this)) { }

        /* Move constructor */
        base_exception(base_exception&& other) : message(std::move(other.message)), file(std::move(other.file)), line(std::move(other.line)),
                                                      function(std::move(other.function)), message_builder(new base_exception_message_builder(*this)) { }

        /* Destructor that cannot throw exception (required by some compilers).
         */
        virtual ~base_exception() throw() { }

        /* Function called when this exception is thrown.
         *
         * @return Error message.
         */
        virtual const char *what() const throw()
        {
            return message_builder->message().c_str();
        }

        /* Retrieve the line number that threw the exception */
        virtual int throw_line() const { return line; }
        /* Retrieve the name of the file that threw the exception */
        virtual std::string throw_file() const { return file; }
        /* Retrieve the signature of the function that threw the exception */
        virtual std::string throw_function() const { return function; }

    protected:
        /** Exception message. */
        std::string message;
        /** Name of the file in which exception has occurred. */
        std::string file;
        /** Name of the function in which exception has occurred. */
        std::string function;
        /** Line at which exception has occurred. */
        int line;

        /* Function responsible for providing the right exception tag (used in <what> result)
         *
         * @return Exception class tag.
         */
        virtual const std::string &tag() const
        {
            const static std::string tag = "BaseException";
            return tag;
        }
    };
}
#endif //MDLUTILS_EXCEPTIONS_BASE_EXCEPTION_HPP
