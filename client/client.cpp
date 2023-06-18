#include "client_utils/utils.h"
#include <boost/asio.hpp>
#include <chrono>
#include <cxxopts.hpp>
#include <deque>
#include <iostream>
#include <rapidjson/document.h>
#include <string>
#include <thread>
//using namespace std::chrono;

using boost::asio::ip::tcp;

//should be constexpr
static const uint32_t samples_window_span(100);
static const uint32_t vector_to_matrix(samples_window_span);
static uint64_t total_vectors_arrived(0);
std::vector<double> single_matrix_arrivals(vector_to_matrix);

std::deque<tp> time_of_arrivals;
std::vector<double> deltas_of_arrivals(samples_window_span);
std::vector<double> curr_vector;
std::vector<std::vector<double>> curr_matrix;
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

    // initialize queue with samples_window_span elements
    tp now_tp = get_current_tp();
    double delta_in_microseconds = convert_hz_to_microseconds(frequency);
    microseconds duration_to_add(static_cast<long long>(delta_in_microseconds));
    std::chrono::microseconds total_duration =
        duration_to_add * samples_window_span;

    for (int i = 0; i < samples_window_span; ++i)
    {
        time_of_arrivals.push_front(now_tp += duration_to_add);
    }

    std::this_thread::sleep_for(total_duration);

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

        // Store the received chunks
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


            // Check if the response data ends with a complete JSON object
            size_t lastBracketIndex = response_data.rfind('}');
            if (lastBracketIndex != std::string::npos)
            {
                std::string completeJsonData =
                    response_data.substr(0, lastBracketIndex + 1);

                // Parse the JSON object
                rapidjson::Document document;
                document.Parse(completeJsonData.c_str());

                // Process the parsed JSON data
                if (!document.HasParseError() && document.IsObject())
                {
                    if (document.HasMember("data") &&
                        document["data"].IsArray())
                    {
                        const rapidjson::Value &dataArray = document["data"];
                        std::vector<double> receivedData;

                        for (rapidjson::SizeType i = 0; i < dataArray.Size();
                             i++)
                        {
                            const rapidjson::Value &data = dataArray[i];
                            if (data.IsDouble())
                            {
                                double value = data.GetDouble();
                                receivedData.push_back(value);
                            }
                        }

                        // Print the received data
                        std::cout << "Received data: ";
                        for (const double &value : receivedData)
                        {
                            std::cout << value << " ";
                            curr_vector.push_back(value);
                        }
                        std::cout << std::endl;
                        auto current_tp = get_current_tp();
                        auto prev_tp = time_of_arrivals.front();
                        auto olders_tp = time_of_arrivals.back();

                        time_of_arrivals.push_front(current_tp);
                        time_of_arrivals.pop_back();

                        //log_results(current_tp, prev_tp, olders_tp, samples_window_span);
                        log_results(current_tp, prev_tp, olders_tp);

                        deltas_of_arrivals.push_back(
                            get_duration(current_tp, prev_tp));
                        total_vectors_arrived++;
                        curr_matrix.push_back(curr_vector);
                        curr_vector.clear();
                        if (0 == total_vectors_arrived % vector_to_matrix)
                        {
                            auto last_matrix_arrival_mean =
                                calculate_mean(deltas_of_arrivals);
                            auto last_matrix_arrival_deviation =
                                calculate_standard_deviation(
                                    deltas_of_arrivals,
                                    last_matrix_arrival_mean);
                            std::cout << "matrix statistics: arrival mean:"
                                      << last_matrix_arrival_mean
                                      << " arrival deviation: "
                                      << last_matrix_arrival_deviation << "\n";
                            deltas_of_arrivals.clear();
                            // calculare mean and standard veriation of the matrix;
                            curr_matrix.clear();
                        }
                    }
                }


                // Clear the response data after processing
                response_data.clear();
            }
        }

        if (error != boost::asio::error::eof)
        {
            throw boost::system::system_error(error);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
