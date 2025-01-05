/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDP Network
*/

#include "spdlog/spdlog.h"
#include "UDPNetwork.hpp"
#include <RType/Network/Packets/EPacketCode.hpp>
#include "RType/ModeManager/ModeManager.hpp"
#include "Packets.hpp"

namespace rtype::network {

    UDPNetwork::UDPNetwork(unsigned short port) : _port(port) {
        this->_ioContext.restart();

        if (IS_SERVER) {
            this->_socket = std::make_shared<asio::ip::udp::socket>(this->_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
        } else {
            this->_socket = std::make_shared<asio::ip::udp::socket>(this->_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
            try {
                this->_serverEndpoint = asio::ip::udp::endpoint(asio::ip::make_address("127.0.0.1"), port);
            } catch (std::exception &e) {
                spdlog::error("Error while creating server endpoint", port);
            }
        }
    }

    void UDPNetwork::start() {
        int numThreads = 2;
        this->_threadPool.emplace(numThreads);

        if (IS_SERVER) {
            this->startReceive();
            spdlog::info("Server UDP network started on port: {}", this->_port);
        } else {
            PacketConnect packet;
            this->sendPacket(packet, _serverEndpoint);
            this->startReceive();
        }

        for (int i = 0; i < numThreads; i++) {
            this->_threadPool->addTask([this] {
                try {
                    this->_ioContext.run();
                } catch (std::exception &e) {
                    spdlog::error("Exception in a UDP IO Thread: {}", e.what());
                }
            });
        }
    }

    UDPNetwork::~UDPNetwork() = default;

    void UDPNetwork::startReceive() {
        auto buffer = std::make_shared<std::vector<char>>(BUFFER_SIZE);

        this->_socket->async_receive_from(asio::buffer(*buffer), this->_endpoint,
        [this, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                handleClient(this->_endpoint, *buffer);
                startReceive();
            } else {
                spdlog::error("Receive error: {}", error.message());
            }
        });
    }

    void UDPNetwork::handleClient(const asio::ip::udp::endpoint &endpoint, const std::vector<char> &buffer) {
        std::string address = endpoint.address().to_string();
        int port = endpoint.port();

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

            if (code == EPacketCode::CONNECT) {
                //TODO: send welcome for the client to know he is connected successfully
            }

        } catch (std::exception &e) {
            spdlog::error(e.what());
        }
    }

    void UDPNetwork::sendPacket(const IPacket &packet, const asio::ip::udp::endpoint &endpoint) const {
        const auto packetData = std::make_shared<std::vector<char>>(packet.bufferize());
        const auto code = packet.getCode();

        _socket->async_send_to(asio::buffer(*packetData), endpoint, [this, code, packetData, endpoint](const asio::error_code& ec, std::size_t) {
            if (ec) {
                spdlog::error("Error while sending message: {}", ec.message());
            } else {
                std::string address = endpoint.address().to_string();
                int port = endpoint.port();
                std::string codeStr = std::to_string(code);

                spdlog::info("Packet: {} successfully sent to: {}:{}", codeStr, address, port);
            }
        });
    }
}
