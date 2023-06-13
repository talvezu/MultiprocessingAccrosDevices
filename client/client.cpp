#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_context io_context;

        // Resolve the server address
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve("127.0.0.1", "8000");

        // Create and connect the socket
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Send the POST request to the server
        std::string request = "POST /enable_stream?client_id=1 HTTP/1.1\r\n";
        request += "Host: 127.0.0.1\r\n";
        request += "Content-Length: 0\r\n";
        request += "Connection: close\r\n\r\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        // Process the streaming response
        std::string data;
        boost::system::error_code error;
        boost::asio::streambuf response;
        while (boost::asio::read(socket,
                                 response,
                                 boost::asio::transfer_at_least(1),
                                 error))
        {
            std::string line;
            std::istream response_stream(&response);
            while (std::getline(response_stream, line))
            {
                data += line + "\n";
                std::cout << line << "\n";
            }

            if (error == boost::asio::error::eof)
                break;
        }

        // Print the received data
        std::cout << data << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
