/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TCP Network
*/

#include "spdlog/spdlog.h"
#include "TCPNetwork.hpp"

#include "RType/ModeManager/ModeManager.hpp"
#include "Packets.hpp"
#include "RType/Config/Config.hpp"

namespace rtype::network {

    //TODO: numThread should be configurable in .ini

    TCPNetwork::TCPNetwork(unsigned short port) : _port(port), _ioContext() {
        if (IS_SERVER) {
            this->_acceptor = asio::ip::tcp::acceptor(this->_ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
        } else {
            const auto address = Config::getInstance().getNetwork().server.address;

            this->_socket = std::make_shared<asio::ip::tcp::socket>(this->_ioContext);
            try {
                asio::ip::tcp::endpoint serverEndpoint(asio::ip::make_address(address), port);
                this->connect(serverEndpoint);
                this->handleClient(this->_socket);
            } catch (std::exception &e) {
                spdlog::error("Unable to connect to TCP socket at {}:{}: {}", address, port, e.what());
            }
        }
    }


    void TCPNetwork::start() {
        int numThreads = 2;
        this->_threadPool.emplace(numThreads);

        if (IS_SERVER) {
            this->startAccept();

            spdlog::info("Server's TCP socket started on port {}", this->_port);
        } else {
            PacketConnect packet;
            this->sendPacket(packet);
        }

        for (int i = 0; i < numThreads; i++) {
            this->_threadPool->addTask([this] {
                try {
                    while (!this->_ioContext.stopped()) {
                        this->_ioContext.run_one();
                    }
                } catch (std::exception &e) {
                    spdlog::error("Exception in a TCP IO Thread: {}", e.what());
                }
            });
        }
        this->_started = true;
    }

    TCPNetwork::~TCPNetwork() {
        this->setStop(true);
        this->_socket->close();
    };


    void TCPNetwork::startAccept() {
        auto socket = std::make_shared<asio::ip::tcp::socket>(this->_ioContext);
        this->_acceptor->async_accept((*socket), [this, socket](const asio::error_code &error) {
            if (!error) {
                std::string address = socket->remote_endpoint().address().to_string();
                int port = socket->remote_endpoint().port();

                spdlog::info("New client connected to TCP socket: {}:{}", address, port);
                handleClient(socket);
            } else {
                spdlog::error("Unable to accept connection on TCP socket: {}", error.message());
            }
            this->startAccept();
        });
    }

    void TCPNetwork::handleClient(std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto buffer = std::make_shared<std::vector<char>>(BUFFER_SIZE);

        socket->async_read_some(asio::buffer(*buffer),
    [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!socket->is_open()) {
                spdlog::warn("TCP socket closed before reading operation could be performed");
                return;
            }
        std::string address = socket->remote_endpoint().address().to_string();
        int port = socket->remote_endpoint().port();
            if (!error) {
                handlePacket(*buffer, socket);
                handleClient(socket);
            } else {
                // On Linux, the error code is `asio::error::eof`, but on Windows, it's `asio::error::connection_reset`
                if (error == asio::error::eof || error == asio::error::connection_reset) {
                    spdlog::info("Remote {}:{} closed the connection on TCP socket", address, port);
                    this->_onPlayerDisconnect(socket);
                } else {
                    spdlog::error("Unable to read client: {}", error.message());
                }
            }
        });
    }

    void TCPNetwork::connect(const asio::ip::tcp::endpoint endpoint) {
        this->_socket->async_connect(endpoint, [this](const asio::error_code& ec) {
            if (ec) {
                spdlog::error("Unable to connect to TCP socket: {}", ec.message());
            }
        });
    }

    void TCPNetwork::sendPacket(const IPacket &packet, std::shared_ptr<asio::ip::tcp::socket> socket) const {
        const auto packetData = std::make_shared<std::vector<char>>(packet.bufferize());
        const auto code = packet.getCode();

        auto targetSocket = socket ? socket : this->_socket;

        if (!targetSocket || !targetSocket->is_open())
            return;

        async_write(*targetSocket, asio::buffer(*packetData), [this, code, targetSocket, packetData](const
        asio::error_code& ec,
        std::size_t) {
            if (ec) {
                spdlog::error("Unable to send message on TCP socket: {}", ec.message());
            } else {
                spdlog::debug("TCP packet [{}] successfully sent", std::to_string(code));
            }
        });
    }

    void TCPNetwork::handlePacket(const std::vector<char> &buffer, std::shared_ptr<asio::ip::tcp::socket> socket)
    const {
        std::string address = socket->remote_endpoint().address().to_string();
        int port = socket->remote_endpoint().port();

        if (buffer.size() < 4) {
            spdlog::error("Invalid TCP packet received from {}:{}", address, port);
            return;
        }

        try {
            std::unique_ptr<IPacket> packet = PacketFactory::fromBuffer(buffer);
            spdlog::debug("TCP packet [{}] received from {}:{}", std::to_string(packet->getCode()), address, port);

            for (auto &handler : this->_handlers) {
                if (handler.first == packet->getCode()) {
                    handler.second(std::move(packet), socket);
                    return;
                }
            }

            for (auto &handler: this->_netHandlers) {
                if (handler.first == packet->getCode()) {
                    handler.second->handle(std::move(packet), socket);
                    return;
                }
            }

            spdlog::warn("No handler found for packet [{}]", std::to_string(packet->getCode()));
        } catch (std::exception &e) {
            spdlog::error("Unable to handle TCP packet: {}", e.what());
        }
    }

    void TCPNetwork::addHandler(EPacketCode code, std::function<void(std::unique_ptr<IPacket>,
        std::shared_ptr<asio::ip::tcp::socket> socket)> handler) {
        this->_handlers.push_back({code, handler});
    }

    void TCPNetwork::setStop(bool state) {
        std::lock_guard<std::mutex> lock(this->_stopMutex);
        this->_stop = state;

        if (state) {
            this->_ioContext.stop();
        }
    }

    bool TCPNetwork::getStop() {
        std::lock_guard<std::mutex> lock(this->_stopMutex);
        return this->_stop;
    }

    TCPNetwork &TCPNetwork::getInstance() {
        static TCPNetwork instance;

        return instance;
    }


}
