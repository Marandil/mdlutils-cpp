#include <iostream>

#include <mdlutils/typedefs.hpp>
#include <mdlutils/exceptions.hpp>

int main()
{
    try
    {
        std::cout << "Hello, World!" << std::endl;
        mdl_throw(mdl::not_implemented_exception, "main");
    }
    catch (std::exception &ex)
    {
        std::cout << "Catched an exception " << ex.what() << std::endl;
    }
    return 0;
}