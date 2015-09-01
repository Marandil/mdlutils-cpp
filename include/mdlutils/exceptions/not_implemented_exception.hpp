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
    class not_implemented_exception : public base_exception
    {
    public:
        not_implemented_exception(const std::string &functionName, const std::string &file, int line) :
                base_exception(functionName, file, line) { commit_message(); }

    protected:
        /// @inherit
        virtual const std::string &getTag() const
        {
            const static std::string tag = "NotImplementedException";
            return tag;
        }
    };
}

#endif //MDLUTILS_EXCEPTIONS_NOT_IMPLEMENTED_EXCEPTION_HPP
