/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDPServer.cpp
*/

#include "spdlog/spdlog.h"
#include "UDPNetwork.hpp"
#include <asio/steady_timer.hpp>

#include "RType/Config/Config.hpp"

//TODO: use packets interface structures for binary data sending and receiving instead of strings
//TODO: use mutexs to protect queues and clients
namespace rtype::network {
    UDPNetwork::StartException::StartException() {}

    UDPNetwork::UDPNetwork(ushort port) : _port(port) {}

    void UDPNetwork::start() {
        if (_running)
            throw StartException();

        try {
            std::string agentType = "Server";

            _ioContext.restart();
            _threadPool.emplace(2);
        #ifdef RTYPE_IS_SERVER
            _socket = asio::ip::udp::socket(_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), _port));
            _receive();
            _send();
        #else
            _socket = asio::ip::udp::socket(_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
            agentType = "Client";
        #endif

            _running = true;

            _threadPool->addTask([this] {
                try {
                    _ioContext.run();
                } catch (const std::exception& e) {
                    spdlog::error("Exception in IO thread: {}", e.what());
                }
            });

            spdlog::info("{} started and running on port {}", agentType, _socket->local_endpoint().port());

        } catch (const std::exception& e) {
            spdlog::error("Failed to start UDP server: {}", e.what());
            throw StartException();
        }

    #ifdef RTYPE_IS_CLIENT
        std::string serverIp = Config::getInstance().getNetwork().server.address;
        spdlog::info("Going to connect server: {}", serverIp);
        this->_connect(serverIp, _port);
    #endif

    }

    void UDPNetwork::stop() {
        _running = false;

        _socket->close();
        _socket.reset();
        _endpoint.reset();
        _ioContext.stop();

        if (_thread.joinable())
            _thread.join();

        spdlog::info("Server stopped");
    }

    void UDPNetwork::_receive() {
        _socket->async_receive_from(
            asio::buffer(_buffer),
            *_endpoint,
            [this](std::error_code errorCode, std::size_t bytesReceived) {
                if (!_running)
                    return;

                if (!errorCode && bytesReceived > 0) {
                    _newPacket(bytesReceived);
                }

                if (errorCode) {
                    spdlog::error("Cannot receive data via UDP server: {}", errorCode.message());
                }

                if (bytesReceived <= 0) {
                    spdlog::warn("UDP server received an invalid number of bytes ({})", bytesReceived);
                }

                _receive();
                _send();
            }
        );
    }

    void UDPNetwork::_send() {
        if (_packetsToSend.empty())
            return;

        std::string to_send = _packetsToSend.front().second;


        std::vector<asio::ip::udp::endpoint> clientsCopy;
        for (const auto& client : _clients) {
            clientsCopy.push_back(client.endpoint);
        }

        for (const auto& endpoint : clientsCopy) {
            _socket->async_send_to(
            asio::buffer(to_send),
            endpoint,
            [this, endpoint](std::error_code ec, std::size_t) mutable {
                if (ec) {
                    spdlog::warn("Client at {} is no longer available: {}", endpoint.address().to_string(), ec.message());
                    _removeClient(endpoint);
                }
            });
        }
        _packetsToSend.pop();
    }

    UDPNetwork &UDPNetwork::getInstance(ushort port) {
        static UDPNetwork server(port);

        return server;
    }

    //TODO: add ifdef for client only (also for _isConnected var)
    //TODO: throw custom appropriate exception
    //TODO: Make max seconds timeout delay configurable in config.ini
    void UDPNetwork::_connect(std::string &ip, ushort port) {
        try {
            if (!_socket.has_value() || !_socket->is_open()) {
                throw std::runtime_error("Server sockedt is not initialized or is closed.");
            }

            asio::ip::udp::endpoint serverEndpoint(asio::ip::address::from_string(ip), port);
            std::string message = "CONNECT";
            asio::error_code ec;
            _socket->send_to(asio::buffer(message), serverEndpoint, 0, ec);

            if (ec) {
                spdlog::error("Failed to send connection request: {}", ec.message());
                return;
            }

            char reply[1024];
            auto start = std::chrono::steady_clock::now();
            int64_t lastElapsed = -1;

            while (std::chrono::steady_clock::now() - start < std::chrono::seconds(5)) {

                int64_t elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - start
                ).count();

                if (elapsed != lastElapsed) {
                    spdlog::info("Waiting for server response... ({}/5 seconds)", elapsed);
                    lastElapsed = elapsed;
                }

                _socket->non_blocking(true);
                size_t reply_length = _socket->receive_from(
                    asio::buffer(reply), 
                    serverEndpoint, 
                    0, 
                    ec
                );
                
                if (ec == asio::error::would_block) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }

                if (!ec && reply_length > 0) {
                    std::string replyMessage(reply, reply_length);
                    _socket->non_blocking(false);
                    if (replyMessage == "OK") {
                        spdlog::info("Successfully connected to server after {} seconds", elapsed);
                        _isConnected = true;
                        break;
                    }
                }
            }

            if (!_isConnected) {
                spdlog::error("Connection timeout after 5 seconds");
                throw std::runtime_error("Connection timeout: Server did not respond");
            }

        } catch (const std::exception& e) {
            spdlog::error("Connection failed: {}", e.what());
            _isConnected = false;
            throw;
        }
    }

    void UDPNetwork::_newPacket(std::size_t bytesReceived) {
        asio::ip::udp::endpoint senderEndpoint = *_endpoint;
        std::string received(_buffer.data(), bytesReceived);

        bool newClient = true;
        std::shared_ptr<ClientInfo> info_ptr;
        for (const auto& client : _clients) {
            if (client.endpoint == senderEndpoint) {
                newClient = false;
            }
        }
        if (newClient) {
            ClientInfo newInfo;
            newInfo.endpoint = senderEndpoint;
            _clients.emplace_back(newInfo);
            info_ptr = std::make_shared<ClientInfo>(newInfo);
        }

        std::pair toHandle = {info_ptr, received};
        _packetsToHandle.emplace(toHandle);

        //TODO: maybe put this into a system
        #ifndef RTYPE_IS_CLIENT
            if (received == "CONNECT") {
                std::pair<std::shared_ptr<ClientInfo>, std::string> toSend = {nullptr, "OK"};
                this->_packetsToSend.emplace(toSend);
            }
        #endif

    }

    void UDPNetwork::_removeClient(asio::ip::udp::endpoint e) {
        auto it = std::find_if(_clients.begin(), _clients.end(), [&](const ClientInfo& client) {
            return client.endpoint == e;
        });

        if (it != _clients.end()) {
            _clients.erase(it);
            spdlog::info("Client at {} removed", e.address().to_string());
        } else {
            spdlog::warn("Client at {} not found", e.address().to_string());
        }
    }
}
