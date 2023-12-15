#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("localhost", "12345");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::string message = "Hello, server!\n";
        boost::asio::write(socket, boost::asio::buffer(message));

        boost::asio::streambuf receive_buffer;
        boost::asio::read_until(socket, receive_buffer, '\n');

        std::istream is(&receive_buffer);
        std::string received_data;
        std::getline(is, received_data);

        std::cout << "Received from server: " << received_data << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
