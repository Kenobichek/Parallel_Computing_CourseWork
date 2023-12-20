#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
	public:
		WebSocketSession(asio::ip::tcp::socket&& socket)
			: ws(std::move(socket)) {}

		void Start();
		void Read();
		void Write();

	private:
		websocket::stream<asio::ip::tcp::socket> ws;
		beast::flat_buffer buffer;
};

class WebSocketServer {
	public:
		WebSocketServer(asio::io_context& io_context, short port)
			: acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
			socket(io_context) {
			HandleClient();
		}

	private:
		asio::ip::tcp::acceptor acceptor;
		asio::ip::tcp::socket socket;

		void HandleClient();
};
