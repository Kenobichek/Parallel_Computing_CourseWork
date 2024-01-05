#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "FileScanner.h"
#include "Server.h"
#include "Timer.h"

using boost::asio::ip::tcp;

int main() {
    boost::property_tree::ptree config;
    boost::property_tree::read_json("./Config.json", config);

    std::string ip = config.get<std::string>("IP");
    int port = config.get<int>("PORT");
    std::string scan_path = config.get<std::string>("SCAN_PATH");

    auto timer = Timer::GetInstance();
    timer->ResetTime();

    InvertedIndex inverted_index;
    FileScanner file_scanner(scan_path, inverted_index);
    file_scanner.ParallelFileProcessing();

    std::printf("Filling Time: %f\n\n", timer->ElapsedTime());

    try {
        boost::asio::io_context io_context;
        WebSocketServer server(io_context, port, inverted_index);
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
