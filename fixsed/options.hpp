#ifndef fixtools_fixsed_options_hpp
#define fixtools_fixsed_options_hpp

#include <iostream>
#include <vector>
#include <optional>

class options
{
public:

    bool parse(int argc, const char** argv);
    void usage(std::ostream& os);

    bool help() const { return m_help; }
    const std::optional<std::string>& in_host() const { return m_in_host; }
	int in_port() const { return m_in_port; }
	const std::string&	out_host() const { return m_out_host; }
	int out_port() const { return m_out_port; }
	const std::optional<std::string>& bind_host() const { return m_bind_host; }
	std::optional<int> bind_port() const { return m_bind_port; }

private:

    void process_in();
    void process_out();
    void process_bind();

    std::string m_program;

    // Raw options from the command line.
    std::string m_in;
    std::string m_out;
    std::string m_bind;

    // Post processed options to make life easier for the caller.
    bool m_help = false;

    std::optional<std::string> m_in_host;
    int m_in_port;

	std::string	m_out_host;
    int m_out_port;

	std::optional<std::string> m_bind_host;
	std::optional<int> m_bind_port;

};

#endif
