#ifndef fixtools_fixsed_options_hpp
#define fixtools_fixsed_options_hpp

#include <iostream>
#include <vector>

class options
{
public:

    bool parse(int argc, const char** argv);
    void usage(std::ostream& os);

    bool help() const;

private:

    std::string m_program;
    bool m_help = false;

};

#endif
