#include <iostream>
#include "options.hpp"

int main(int argc, const char** argv)
{
    try
    {
        options options;

        if (!options.parse(argc, argv) || options.help())
        {
            options.usage(std::cerr);
            return 1;
        }

         
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}