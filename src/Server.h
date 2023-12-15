#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class Server {
	public:
		Server(boost::asio::io_context& io_context, short port)
			: acceptor(io_context, tcp::endpoint(tcp::v4(), port)),
			socket(io_context) {
			HandleClient();
		}

	private:
		void Read();
		void Write();
		void HandleClient();

		tcp::acceptor acceptor;
		tcp::socket socket;
		boost::asio::streambuf receive_buffer;
};
