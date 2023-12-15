#include "Server.h"

void Server::Read() {
	boost::asio::async_read_until(
		socket,
		receive_buffer,
		'\n',
		[this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
			if (!ec) {
				std::istream is(&receive_buffer);
				std::string received_data;
				std::getline(is, received_data);
				std::cout << "Received from client:: " << received_data << std::endl;
				socket.close();
			} else {
				std::cerr << "Error reading from client: " << ec.message() << std::endl;
			}
		}
	);
}


void Server::Write() {
	std::string message = "Hello Client!!!\n";
	boost::asio::write(socket, boost::asio::buffer(message));
}


void Server::HandleClient() {
	acceptor.async_accept(
		socket,
		[this](const boost::system::error_code& ec) {
			if (!ec) {
				std::cout << "Client connected!" << std::endl;
				Write();
				Read();
			}
			HandleClient();
		}
	);
}
