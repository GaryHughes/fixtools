#ifndef fixtools_utilities_print_fix_message_hpp
#define fixtools_utilities_print_fix_message_hpp

#include <iostream>
#include <fix8/f8includes.hpp>

namespace utility
{

void print_fix_message(const FIX8::Message& message, std::ostream& os);

}

#endif