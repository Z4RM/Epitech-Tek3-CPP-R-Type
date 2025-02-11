/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDP Network
*/

#include "spdlog/spdlog.h"
#include "UDPNetwork.hpp"
#include <Network/Packets/EPacketCode.hpp>
#include "RType/ModeManager/ModeManager.hpp"
#include "Packets.hpp"
#include "RType/Config/Config.hpp"

namespace rtype::network {

    UDPNetwork::UDPNetwork(unsigned short port) : _port(port), _ioContext() {
        if (IS_SERVER) {
            this->_socket = std::make_shared<asio::ip::udp::socket>(this->_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
        } else {
            this->_socket = std::make_shared<asio::ip::udp::socket>(this->_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
            try {
                this->_serverEndpoint = asio::ip::udp::endpoint(asio::ip::make_address(Config::getInstance().getNetwork().server.address), port);
            } catch (std::exception &e) {
                spdlog::error("Unable to create server endpoint: {}", e.what());
            }
        }
    }

    void UDPNetwork::start() {
        int numThreads = 2;
        this->_threadPool.emplace(numThreads);

        if (IS_SERVER) {
            this->startReceive();
            spdlog::info("Server's UDP socket started on port {}", this->_port);
        } else {
            this->startReceive();
        }

        for (int i = 0; i < numThreads; i++) {
            this->_threadPool->addTask([this] {
                try {
                    this->_ioContext.run();
                } catch (std::exception &e) {
                    spdlog::error("Exception in an UDP IO Thread: {}", e.what());
                }
            });
        }
        this->_started = true;
    }

    UDPNetwork::~UDPNetwork() {
        this->setStop(true);
    }

    void UDPNetwork::startReceive() {
        auto buffer = std::make_shared<std::vector<char>>(BUFFER_SIZE);

        this->_socket->async_receive_from(asio::buffer(*buffer), this->_endpoint,
        [this, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                handlePacket(this->_endpoint, *buffer);
                startReceive();
            } else {
                spdlog::error("Unable to receive data on UDP socket: {}", error.message());
            }
        });
    }

    void UDPNetwork::handlePacket(const asio::ip::udp::endpoint &endpoint, const std::vector<char> &buffer) {
        std::string address = endpoint.address().to_string();
        int port = endpoint.port();

        if (buffer.size() < 4) {
            spdlog::error("Invalid UDP packet received from {}:{}", address, port);
            return;
        }
        try {
            std::unique_ptr<IPacket> packet = PacketFactory::fromBuffer(buffer);
            std::string codeStr = std::to_string(packet->getCode());
            for (auto& handler : _handlers) {
                if (handler.first == packet->getCode()) {
                    handler.second(std::move(packet), endpoint);
                    return;
                }
            }

            auto it = this->_netHandlers.find(packet->getCode());
            if (it != this->_netHandlers.end()) {
                it->second->handle(std::move(packet), endpoint);
                return;
            }

            spdlog::warn("No handler found for packet [{}]", codeStr);
        } catch (std::exception &e) {
            spdlog::error("Unable to handle UDP packet: {}", e.what());
        }
    }

    void UDPNetwork::sendPacket(const IPacket &packet, const asio::ip::udp::endpoint &endpoint) const {
        const auto packetData = std::make_shared<std::vector<char>>(packet.bufferize());
        const auto code = packet.getCode();

        _socket->async_send_to(asio::buffer(*packetData), endpoint, [this, code, packetData, endpoint](const asio::error_code& ec, std::size_t) {
            if (ec) {
                spdlog::error("Unable to send UDP packet: {}", ec.message());
            } else {
                std::string address = endpoint.address().to_string();
                int port = endpoint.port();
                std::string codeStr = std::to_string(code);
            }
        });
    }

    void UDPNetwork::addHandler(EPacketCode code, std::function<void(std::unique_ptr<IPacket>, asio::ip::udp::endpoint endpoint)>
    handler) {
        this->_handlers.push_back({code, handler});
    }

    void UDPNetwork::setStop(bool state) {
        if (state) {
            this->_ioContext.stop();
        }
    }

    UDPNetwork &UDPNetwork::getInstance(unsigned short port) {
        static UDPNetwork instance(port);

        return instance;
    }
}
