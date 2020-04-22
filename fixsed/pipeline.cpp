#include "pipeline.hpp"
#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <fix8/f8includes.hpp>
#include "libschema/FIX50SP2_types.hpp"
#include "libschema/FIX50SP2_router.hpp"
#include "libschema/FIX50SP2_classes.hpp"
#include "libutility/print_fix_message.hpp"

using boost::asio::ip::tcp;
using namespace FIX8;
using namespace utility;

const char* fix_message_prefix = "8=FIX";

void process_messages(tcp::socket& read_socket, 
                      tcp::socket& write_socket, 
                      const std::string& read_label,
                      const std::string& write_label)
{
    // This is all a bit gross and inefficient but its enough to learn the FIX8 API.
    // FIX8 needs a decoupled, restartable, streaming parser.
    try
    {
        auto data = std::vector<char>(8192);
        size_t offset {0};
        auto fix_message_prefix_length = strlen(fix_message_prefix);

        while (true)
        {
            if (offset == data.size())
            {
                // The buffer is full but it's not an entire message.
                data.resize(data.size() * 1.5);
            }

            // This is either our first read at the start of the buffer or we have previously
            // performed a partial read that did not finish on a clean message boundary.
            auto buffer = boost::asio::buffer(&*data.begin() + offset, 
                                              data.size() - offset);

            boost::system::error_code error;
          
            size_t bytes_read = read_socket.read_some(buffer, error);
          
            if (error == boost::asio::error::eof)
            {
                break;
            }
            
            if (error)
            {
                throw boost::system::system_error(error);
            }

            offset += bytes_read;
            // Terminate the data we have because we use strstr below and if the buffer still has a previous
            // message in it then the checksum find will succeed when it shouldn't. 
            data[offset] = 0;

            // Walk through the buffer and parse any complete messages.
            auto current = &*data.begin();
            auto end = current + offset;

            while (current < end)
            {
                if ((end - current) < fix_message_prefix_length)
                {
                    // We haven't read enough for a message prefix yet
                    break;
                }

                if (strncmp(&*data.begin(), fix_message_prefix, fix_message_prefix_length))
                {
                    // We should always be at the start of a message here so something is broken.
                    throw std::runtime_error("did not find message prefix at current buffer position");
                }

                auto checksum_pos = std::strstr(current, "10=");

                if (checksum_pos == NULL)
                {
                    // We don't have a complete message yet.
                    break;
                }

                // check we have enough bytes here for + 7

                auto message_text = std::string(current, checksum_pos + 7);

                // TODO - improve the diagnostics in this function particularly when the checksum bit fails @ line 259
                // TODO - setting permissive=true causes the CheckSum to be put into the _unknown collection which is
                //        copied blindly when encoding so we end up with 2 checksums. 
                std::unique_ptr<Message> message(Message::factory(FIX8::FIX50SP2::ctx(), message_text, false, false));

                if (!message)
                {
                    throw std::runtime_error("message parse failed [" + message_text + "]");
                }

                print_fix_message(*message, std::cout);
                std::cout << std::endl;

                std::string out;
                message->Trailer()->set(Common_CheckSum, FieldTrait::suppress);
                message->encode(out);

                boost::asio::write(write_socket, boost::asio::buffer(out));

                current = checksum_pos + 7;
            }

            if (current != &*data.begin())
            {
                if (current != end)
                {
                    // We read one or more messages and there are still some bytes left.
                    // Move them to the start of the buffer to make the next cyclye simpler and to
                    // reduce the chance of having to grow the buffer.
                    memcpy(&*data.begin(), current, end - current);
                    offset = end - current;
                }
                else
                {
                    // We read one or more messages and there are no bytes left.
                    offset = 0;
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "exception in thread: " << e.what() << "\n";
    }
}

void close_socket(tcp::socket& socket)
{
    try
    {
        if (socket.is_open())
        {
            socket.close();
        }
    }
    catch (std::exception& ex)
    {
        std::cerr << "Error closing socket: " << ex.what() << std::endl;
    }
}

void pipeline::run(const options& options)
{
    try
    {
        boost::asio::io_context io_context;
        
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), options.in_port()));

        // TODO - bind host

        acceptor.listen();
        tcp::socket initiator_socket(io_context);

        std::ostringstream msg;
			  msg << "waiting for initiator [" << (options.in_host() ? *options.in_host() : "*") << ":" << options.in_port() << "]";
			  std::cout << msg.str() << std::endl;

        acceptor.accept(initiator_socket);

        msg.str("");
			  msg << "accepted initiator [" 
            << initiator_socket.remote_endpoint().address().to_string() << ":"
            << initiator_socket.remote_endpoint().port() << "]";
			  std::cout << msg.str() << std::endl;

        msg.str("");
        msg << "resolving acceptor [" << options.out_host() << ":" << options.out_port() << "]";
        std::cout << msg.str() << std::endl;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(options.out_host(), std::to_string(options.out_port()));
        
        msg.str("");
        msg << "connecting to acceptor [" << options.out_host() << ":" << options.out_port() << "]";
        std::cout << msg.str() << std::endl;
  
        tcp::socket acceptor_socket(io_context);

        // TODO - bind

        boost::asio::connect(acceptor_socket, endpoints);

        msg.str("");
			  msg << "connected to acceptor [" 
            << acceptor_socket.remote_endpoint().address().to_string() << ":"
            << acceptor_socket.remote_endpoint().port() << "]";
			  std::cout << msg.str() << std::endl;

        std::thread thread([&] {
            process_messages(acceptor_socket, initiator_socket, "ACCEPTOR", "INIITIATOR");
            close_socket(initiator_socket);
            close_socket(acceptor_socket);
        });

        process_messages(initiator_socket, acceptor_socket, "INITIATOR", "ACCEPTOR");
        close_socket(initiator_socket);
        close_socket(acceptor_socket);

        thread.join();
    }
    catch(std::exception& ex)
    {
        std::cerr << "error: " << ex.what() << std::endl;
    }
}