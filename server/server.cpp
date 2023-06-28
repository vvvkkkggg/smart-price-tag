#include "security.h"
#include "commands.h"
#include "json.hpp"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

// [x] подключаем sqlite
// добавляем все обновления в очередь
// очередь на мьютексе
// одним потоком берем данные и начинаем грузить их в ардуино
// между делом сохраняем значения в БД
// написать логику ручек
// отправка значений на ардуинку (конфиг ip контроллеров)
// авто настрока на старте (секретный ключ, ip адреса)

// ардуино, принимаем данные (настроить скорость)
// их обработка

// на боте написать логику отправки на сервер
// на боте добавить очередь (kafka), чтобы значения в тредпуле потом обработались
// дописать две кнопки + работает ли сервер? + какой сервер подключен? (отедльный ряд)

// схемы
// презентация
// документация, типа тесты какие-то
// узнать что спрашивали на плюсах, хз хз


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