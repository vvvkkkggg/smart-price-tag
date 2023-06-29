#include "security.h"
#include "commands.h"
#include "json.hpp"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "db.h"

void session(boost::asio::ip::tcp::socket sock) {
    try {
        for (;;) {
            std::string data;
            std::vector<char> dataBuffer(1024);

            boost::system::error_code error;

            bool flag = true;
            ssize_t read;

            while ((read = sock.read_some(boost::asio::buffer(dataBuffer), error))) {
                int i = 0;

                if (flag) {
                    std::string secretKey;
                    while (dataBuffer[i] != '\n') { secretKey += dataBuffer[i++]; }
                    i++;
                    flag = false;

                    if (!Security::isSecretKeyValid(secretKey)) {
                        sock.close();
                    }
                }

                while (i != read) {
                    data += dataBuffer[i++];
                }
            }

            sock.write_some(
                    boost::asio::buffer(
                            Command::runCommand(data)
                    )
            );

            if (error == boost::asio::error::eof) {
                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }

    sock.close();
}

[[noreturn]] void server(boost::asio::io_context &io_context, unsigned short port) {
    boost::asio::ip::tcp::acceptor a(
            io_context,
            boost::asio::ip::tcp::endpoint(
                    boost::asio::ip::tcp::v4(), port)
    );

    for (;;) {
        std::thread(session, a.accept()).detach();
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        server(io_context, std::atoi(argv[1]));
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}