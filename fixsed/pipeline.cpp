#include "pipeline.hpp"
#include <iostream>
#include <sstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void process_messages(tcp::socket& read_socket, tcp::socket& write_socket)
{

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
            process_messages(acceptor_socket, initiator_socket);
        });

        process_messages(initiator_socket, acceptor_socket);

        thread.join();
    }
    catch(std::exception& ex)
    {
      std::cerr << "error: " << ex.what() << std::endl;
    }
}