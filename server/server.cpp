#include "request_entity.h"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

Entity parse_entity(const std::vector<char>& buffer);

namespace {
    constexpr size_t BUFFER_SIZE = 128;
}

void session(tcp::socket sock) {
    try {
        for (;;) {
            std::vector<char> data_buffer(BUFFER_SIZE);

            boost::system::error_code error;
            sock.read_some(boost::asio::buffer(data_buffer), error);
            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);

            Entity ent = parse_entity(data_buffer);

            sock.write_some(boost::asio::buffer("Got product name: " + ent.get_product_name() + + "\n"));
            sock.write_some(boost::asio::buffer("Got product cost: " + ent.get_cost() + "\n"));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_context& io_context, unsigned short port) {
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    for (;;) {
        std::thread(session, a.accept()).detach();
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        server(io_context, std::atoi(argv[1]));
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}