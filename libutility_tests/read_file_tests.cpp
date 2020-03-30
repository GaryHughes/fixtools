#include <catch.hpp>
#include <libutility/read_file.hpp>

using namespace utility;

TEST_CASE( "read_file", "[read_file]" ) {

    SECTION( "reading a non existent file returns a stream in an error state" ) {
        read_file("/no_such_file", 
            [&](std::istream& is)
            {
                REQUIRE(!is);
            });    
    }

}