#include "options.hpp"
#include <boost/program_options.hpp>
#include <libgen.h>

namespace po = boost::program_options;

static const char* option_help = "help";

bool options::parse(int argc, const char** argv)
{
    m_program = argv[0];

    po::options_description options;
    
    options.add_options()
        (option_help, "display usage");

    po::variables_map variables;
    po::store(po::command_line_parser(argc, argv).options(options).run(), variables);
    po::notify(variables);

    if (variables.count(option_help))
    {
        m_help = true;
        return true;
    }

    return true;
}

void options::usage(std::ostream& os)
{
    os << "usage: " << basename(const_cast<char*>(m_program.c_str())) << " [--help]" << std::endl;
}

bool options::help() const
{
    return m_help;
}

