#ifndef fixtools_utilities_read_file_hpp
#define fixtools_utilities_read_file_hpp

#include <iostream>
#include <functional>

namespace utilities
{

void read_file(const std::string& filename, const std::function<void(std::istream& is)>& reader);

}

#endif