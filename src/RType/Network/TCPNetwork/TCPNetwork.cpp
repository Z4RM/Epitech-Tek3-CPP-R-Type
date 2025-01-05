/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TCP Network
*/

#include "spdlog/spdlog.h"
#include "TCPNetwork.hpp"

#include <RType/Network/Packets/EPacketCode.hpp>

#include "RType/ModeManager/ModeManager.hpp"
#include "Packets.hpp"


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
                spdlog::info("Connection established with the server TCP: 127.0.0.1:{}", port);
                this->handleClient(this->_socket);
            } catch (std::exception &e) {
                spdlog::error("Error while establishing a connction to the server tcp network: 127.0.0.1:{}", port);
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
            PacketConnect packet;
            this->sendPacket(packet);
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
        auto buffer = std::make_shared<std::vector<char>>(BUFFER_SIZE);

        socket->async_read_some(asio::buffer(*buffer),
    [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
        std::string address = socket->remote_endpoint().address().to_string();
        int port = socket->remote_endpoint().port();

            if (!error) {
                handlePacket(*buffer, socket);
                handleClient(socket);
            } else {
                if (error == asio::error::eof) {
                    spdlog::warn("Remote {}:{} closed the connection", address, port);
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

    void TCPNetwork::sendPacket(const IPacket &packet, std::shared_ptr<asio::ip::tcp::socket> socket) const {
        const auto packetData = std::make_shared<std::vector<char>>(packet.bufferize());
        const auto code = packet.getCode();
        auto targetSocket = socket ? socket : this->_socket;

        async_write(*targetSocket, asio::buffer(*packetData), [this, code, targetSocket, packetData](const
        asio::error_code& ec,
        std::size_t) {
            if (ec) {
                spdlog::error("Error while sending message: {}", ec.message());
            } else {
                std::string address = targetSocket->remote_endpoint().address().to_string();
                std::string codeStr = std::to_string(code);
                int port = targetSocket->remote_endpoint().port();

                spdlog::info("Packet: {} successfully sended to: {}:{}", codeStr, address, port);
            }
        });
    }

    void TCPNetwork::handlePacket(const std::vector<char> &buffer, std::shared_ptr<asio::ip::tcp::socket> socket)
    const {
        std::string address = socket->remote_endpoint().address().to_string();
        int port = socket->remote_endpoint().port();

        if (buffer.size() < 4) {
            spdlog::error("Invalid packet received from {}:{}", address, port);
            return;
        }
        int code = 0;
        std::memcpy(&code, buffer.data(), sizeof(int));

        try {
            std::unique_ptr<IPacket> packet = PacketFactory::fromCode(code);
            std::string codeStr = std::to_string(code);
            spdlog::info("Packet: {} received from {}:{}", codeStr, address, port);
        } catch (std::exception &e) {
            spdlog::error(e.what());
        }
    }
}
