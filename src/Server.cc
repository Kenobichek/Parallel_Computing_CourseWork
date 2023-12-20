#include "Server.h"

void WebSocketServer::HandleClient() {
	acceptor.async_accept(socket, [this](const beast::error_code& ec) {
		if (!ec) {
			std::cout << "Client connected!" << std::endl;
			std::make_shared<WebSocketSession>(std::move(socket))->Start();
		}
		HandleClient();
	});
}

void WebSocketSession::Start() {
	ws.async_accept([self = shared_from_this()](const beast::error_code& ec) {
		if (!ec) {
			self->Read();
		}
	});
}

void WebSocketSession::Read() {
	ws.async_read(buffer, [self = shared_from_this()](const beast::error_code& ec, std::size_t) {
		if (!ec) {
			std::string received_data(beast::buffers_to_string(self->buffer.data()));
			std::cout << "Received from client: " << received_data << std::endl;

			self->Write();
		} 
		else {
				std::cerr << "Error reading WebSocket frame: " << ec.message() << std::endl;
		}
	});
}

void WebSocketSession::Write() {
	std::string message = "Hello Client!!!";
	ws.text(true);
	ws.async_write(asio::buffer(message), [self = shared_from_this()](const beast::error_code& ec, std::size_t) {
			if (ec) {
					std::cerr << "Error writing WebSocket frame: " << ec.message() << std::endl;
			}
	});
}
