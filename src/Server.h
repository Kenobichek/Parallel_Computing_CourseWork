#pragma once
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "InvertedIndex.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
	public:
		WebSocketSession(asio::ip::tcp::socket&& socket, InvertedIndex& index)
			: ws(std::move(socket)), inverted_index(index) {}

		void Start();
		void Read();
		void Write(const std::string& word);

	private:
		websocket::stream<asio::ip::tcp::socket> ws;
		beast::flat_buffer buffer;

		InvertedIndex& inverted_index;
};

class WebSocketServer {
	public:
		WebSocketServer(asio::io_context& io_context, short port, InvertedIndex& inverted_index)
			: acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
			socket(io_context), inverted_index(inverted_index) {
			HandleClient();
		}

	private:
		asio::ip::tcp::acceptor acceptor;
		asio::ip::tcp::socket socket;

		InvertedIndex& inverted_index;

		void HandleClient();
};
