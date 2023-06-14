#include <boost/asio.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <rapidjson/document.h>
#include <string>


using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    cxxopts::Options options("YourProgram", "Description of your program");

    options.add_options()("client_id",
                          "Client ID",
                          cxxopts::value<std::string>())(
        "frequency",
        "Frequency in Hertz",
        cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    std::string client_id = result["client_id"].as<std::string>();
    std::string frequency = result["frequency"].as<std::string>();
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

        // Construct the form data
        std::string form_data =
            "client_id=" + client_id + "&frequency=" + frequency;

        // Send the POST request to the server
        std::string request = "POST /register_for_vector_stream HTTP/1.1\r\n";
        request += "Host: 127.0.0.1\r\n";
        request +=
            "Content-Length: " + std::to_string(form_data.length()) + "\r\n";
        request += "Content-Type: application/x-www-form-urlencoded\r\n";
        request += "Connection: close\r\n\r\n";
        request += form_data;
        boost::asio::write(socket, boost::asio::buffer(request));
        // Read the response from the server
        boost::system::error_code error;
        boost::asio::streambuf response;
        std::string response_data;

        while (boost::asio::read(socket,
                                 response,
                                 boost::asio::transfer_at_least(1),
                                 error))
        {
            // Process the received data
            response_data +=
                boost::asio::buffer_cast<const char *>(response.data());
            response.consume(response.size());

            // Print the received data
            std::cout << response_data << std::endl;

            // Parse the JSON response
            // Assuming you have a JSON parsing library, you can use it to extract the vector data from the response
            // For example, if you're using a library like RapidJSON:

            rapidjson::Document document;
            document.Parse(response_data.c_str());

            // Access the vector data from the JSON document
            if (document.IsArray())
            {
                for (const auto &value : document.GetArray())
                {
                    if (value.IsNumber())
                    {
                        double number = value.GetDouble();
                        // Process the number as needed
                        std::cout << number << " \n";
                    }
                }
            }
            if (error == boost::asio::error::eof)
                break;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
