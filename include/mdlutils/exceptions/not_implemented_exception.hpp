//
// Created by marandil on 17.08.15.
//

#ifndef MDLUTILS_EXCEPTIONS_NOT_IMPLEMENTED_EXCEPTION_HPP
#define MDLUTILS_EXCEPTIONS_NOT_IMPLEMENTED_EXCEPTION_HPP

#include <string>

#include <mdlutils/typedefs.hpp>
#include <mdlutils/exceptions/base_exception.hpp>

namespace mdl
{
    /* Simple exception that should be thrown in all unimplemented methods.
     *
     * Other uses are welcome.
     */
    class not_implemented_exception : public base_exception
    {
    public:
        /* Default constructor.
         * @file Name of the file file in which exception has occurred.
         * @line Line at which exception has occurred.
         * @function The signature of the function that throws the exception.
         */
        not_implemented_exception(const std::string &file, int line, const std::string &functionName, const std::string& msg = "") :
                base_exception(file, line, functionName, msg) { }

    protected:
        /// @inherit
        virtual const std::string &tag() const
        {
            const static std::string tag = "NotImplementedException";
            return tag;
        }
    };
}

#endif //MDLUTILS_EXCEPTIONS_NOT_IMPLEMENTED_EXCEPTION_HPP
