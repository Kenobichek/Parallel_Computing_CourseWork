#include "Server.h"

void WebSocketServer::HandleClient() {
	acceptor.async_accept(socket, [this](const beast::error_code& ec) {
		if (!ec) {
			std::cout << "Client connected!" << std::endl;
			std::make_shared<WebSocketSession>(std::move(socket), inverted_index)->Start();
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

			self->buffer.consume(self->buffer.size());
			self->Write(received_data);
			self->Read();
		} 
		else {
			if (ec == beast::websocket::error::closed) {
				std::cout << "WebSocket connection closed by the client." << std::endl;
			}
			else {
				std::cerr << "Error reading WebSocket frame: " << ec.message() << std::endl;
			}
		}
	});
}

void WebSocketSession::Write(const std::string& word) {
	boost::property_tree::ptree pt;
	Files files = inverted_index.GetFiles(word);
	boost::property_tree::ptree filesNode;

	for (const auto& filePath : files) {
		boost::property_tree::ptree fileNode;
		fileNode.put_value(filePath.string());
		filesNode.push_back(std::make_pair("", fileNode));
	}
	
	if (!filesNode.empty()) {
		pt.put_child("filePaths", filesNode);
	}

	std::ostringstream oss;
	boost::property_tree::write_json(oss, pt);

	ws.text(true);
	ws.async_write(asio::buffer(oss.str()), 
		[self = shared_from_this()](const beast::error_code& ec, std::size_t) {
			if (ec) {
				std::cerr << "Error writing WebSocket frame: " << ec.message() << std::endl;
			}
		}
	);
}
