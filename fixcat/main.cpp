#include <fix8/f8includes.hpp>
#include "libschema/FIX50SP2_types.hpp"
#include "libschema/FIX50SP2_router.hpp"
#include "libschema/FIX50SP2_classes.hpp"
#include "options.hpp"

using namespace FIX8;

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

        auto message = Message::factory(FIX8::FIX50SP2::ctx(), "", true, true);
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}