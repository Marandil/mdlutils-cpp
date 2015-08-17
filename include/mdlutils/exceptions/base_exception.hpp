//
// Created by marandil on 17.08.15.
//

#ifndef MDLUTILS_CPP_BASE_EXCEPTION_HPP
#define MDLUTILS_CPP_BASE_EXCEPTION_HPP

#include <exception>

#include <mdlutils/typedefs.hpp>

#ifndef mdl_throw
/** Custom exception throwing (adds filename and exception line).
 * @ex exception class
 * @args exception class arguments.
 */
#define mdl_throw(ex, args...) throw ex(args, __FILE__, __LINE__);
#endif

namespace mdl
{

    /** Virtual class for creating custom exceptions.
     */
    class base_exception : public std::exception
    {
    public:
        /**
         * Constructs exception instance.
         * @param file name of the file file in which exception has occurred.
         * @param line line at which exception has occurred.
         */
        base_exception(const std::string &file, int line)
                : message(""), file(file), line(line) { }

        /**
         * Constructs exception instance.
         * @param err exception message.
         * @param file name of the file file in which exception has occurred.
         * @param line line at which exception has occurred.
         */
        base_exception(const std::string &err, const std::string &file, int line)
                : message(err), file(file), line(line) { }

        /**
         * Destructor that cannot throw exception (required by some compilers).
         */
        virtual ~base_exception() throw() { }

        /**
         * Function called when this exception is thrown.
         * @return error message.
         */
        virtual const char *what() const throw()
        {
            return (getTag() + ": " + message + "\n\tat " + std::to_string(line) + " in " + file).c_str();
        }

    protected:
        /** Exception message. */
        std::string message;
        /** Name of the file in which exception has occurred. */
        std::string file;
        int line; /**< Line at which exception has occurred. */

        /**
         * @return Exception class tag.
         */
        virtual const std::string &getTag() const = 0;
    };
}
#endif //MDLUTILS_CPP_BASE_EXCEPTION_HPP
