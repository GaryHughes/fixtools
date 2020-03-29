#include <fix8/f8includes.hpp>
#include "schema/FIX50SP2_types.hpp"
#include "schema/FIX50SP2_router.hpp"
#include "schema/FIX50SP2_classes.hpp"

using namespace FIX8;

int main(int argc, char** argv)
{
    auto message = Message::factory(FIX8::FIX50SP2::ctx(), "", true, true);
	
    return 0;
}