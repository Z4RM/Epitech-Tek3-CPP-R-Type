/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TODO: add description
*/

#include "spdlog/spdlog.h"
#include "TCPNetwork.hpp"
#include "RType/ModeManager/ModeManager.hpp"


namespace rtype::network {

    //TODO: numThread should be configurable in .ini

    TCPNetwork::TCPNetwork(unsigned short port) : _port(port) {
        this->_ioContext.restart();

        if (IS_SERVER) {
            this->_acceptor = asio::ip::tcp::acceptor(this->_ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
        } else {
            this->_socket = std::make_shared<asio::ip::tcp::socket>(this->_ioContext);
            try {
                asio::ip::tcp::endpoint serverEndpoint(asio::ip::make_address("127.0.0.1"), port);
                this->connect(serverEndpoint);
                spdlog::info("Successfully connected to the server tcp network: 127.0.0.1:{}", port);
                this->handleClient(this->_socket);
            } catch (std::exception &e) {
                spdlog::error("Error while connecting to the server tcp network: 127.0.0.1:{}", port);
            }
        }
    }


    void TCPNetwork::start() {
        int numThreads = 2;
        this->_threadPool.emplace(numThreads);

        if (IS_SERVER) {
            this->startAccept();

            spdlog::info("Server tcp network started on port: {}", this->_port);
        } else {
            std::string message = "CONNECT";

            this->sendMessage(message);
        }

        for (int i = 0; i < numThreads; i++) {
            this->_threadPool->addTask([this] {
                try {
                    this->_ioContext.run();
                } catch (std::exception &e) {
                    spdlog::error("Exception in a TCP IO Thread: {}", e.what());
                }
            });
        }
    }

    TCPNetwork::~TCPNetwork() = default;


    void TCPNetwork::startAccept() {
        auto socket = std::make_shared<asio::ip::tcp::socket>(this->_ioContext);
        this->_acceptor->async_accept((*socket), [this, socket](const asio::error_code &error) {
            if (!error) {
                std::string address = socket->remote_endpoint().address().to_string();
                int port = socket->remote_endpoint().port();

                spdlog::info("New client connected: {}:{}", address, port);
                handleClient(socket);
            } else {
                spdlog::error("Accept error: {}", error.message());
            }
            this->startAccept();
        });
    }

    //TODO: try new method to have dynamic buffer size
    void TCPNetwork::handleClient(std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto buffer = std::make_shared<std::vector<char>>(1024);

        socket->async_read_some(asio::buffer(*buffer),
    [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                std::string message(buffer->data(), bytes_transferred);
                std::string address = socket->remote_endpoint().address().to_string();
                int port = socket->remote_endpoint().port();

                spdlog::info("Message: {} received from: {}:{}", message, address, port);
                handleMessage(message, socket);
                handleClient(socket);
            } else {
                if (error == asio::error::eof) {
                    std::cout << "Client closed the connection" << std::endl;
                } else {
                    spdlog::error("Receive error while reading client: {}", error.message());
                }
            }
        });
    }

    void TCPNetwork::connect(const asio::ip::tcp::endpoint& endpoint) {
        this->_socket->async_connect(endpoint, [this](const asio::error_code& ec) {
            if (!ec) {
                spdlog::info("Connected to server");
            } else {
                spdlog::error("Connection failed: {}", ec.message());
            }
        });
    }

    void TCPNetwork::sendMessage(const std::string &message, std::shared_ptr<asio::ip::tcp::socket> socket) const {
        const auto msgPtr = std::make_shared<std::string>(message);
        auto targetSocket = socket ? socket : this->_socket;

        async_write(*targetSocket, asio::buffer(*msgPtr), [this, msgPtr, targetSocket](const asio::error_code& ec, std::size_t) {
            if (ec) {
                spdlog::error("Error while sending message: {}", ec.message());
            } else {
                std::string address = targetSocket->remote_endpoint().address().to_string();
                int port = targetSocket->remote_endpoint().port();
                spdlog::info("Message: {} successfully sended to: {}:{}", *msgPtr, address, port);
            }
        });
    }

    void TCPNetwork::handleMessage(const std::string &message, std::shared_ptr<asio::ip::tcp::socket> socket) const {
        if (message == "CONNECT") {
            std::string msg = "WELCOME";

            this->sendMessage(msg, socket);
        }
    }
}
