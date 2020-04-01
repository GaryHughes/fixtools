#include <fix8/f8includes.hpp>
#include <boost/algorithm/string.hpp>
#include "libschema/FIX50SP2_types.hpp"
#include "libschema/FIX50SP2_router.hpp"
#include "libschema/FIX50SP2_classes.hpp"
#include "options.hpp"
#include "libutility/read_file.hpp"

using namespace utility;
using namespace FIX8;

const char* fix_message_prefix = "8=FIX";

void pretty_print_line(const std::string& line)
{
    auto start_of_message = line.find(fix_message_prefix);

    if (start_of_message == std::string::npos)
    {
        return;
    }

    auto message_text = line.substr(start_of_message);
    boost::algorithm::trim(message_text);

    // fix8 is stricter than we would like for this kind of activity
    // TODO - add the ability to parse partial messages
    auto checksum_pos = message_text.find_last_of('\x01' + "10=");

    if (checksum_pos == std::string::npos)
    {
        return;
    }

    message_text = message_text.substr(0, checksum_pos + 6);

    // TODO - improve the diagnostics in this function particularly when the checksum bit fails @ line 259
    std::unique_ptr<Message> message(Message::factory(FIX8::FIX50SP2::ctx(), message_text, true, true));

    if (message)
    {
        std::cout << *message << std::endl;
    }
}

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

        for (const auto filename : options.input_files())
        {
            read_file(filename, [](std::istream& is)
            {
                for (;;)
                {
                    std::string line;
                    
                    if (!std::getline(is, line))
                    {
                        break;
                    }

                    pretty_print_line(line);
                }
            });
        }
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}