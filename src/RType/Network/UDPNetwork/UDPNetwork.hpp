/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDPServer.hpp
*/

#pragma once

#include <optional>
#include <RType/ThreadPool/ThreadPool.hpp>
#include "asio.hpp"
#include "Packets.hpp"

#define BUFFER_SIZE 1024

namespace rtype::network {

    class UDPNetwork {
    public:
        explicit UDPNetwork(unsigned short port = 0);
        ~UDPNetwork();

        void start();

        //SERVER
        void startReceive();
        void handleClient(const asio::ip::udp::endpoint &endpoint, const std::vector<char> &buffer);

        //CLIENT
        void sendPacket(const IPacket &message, const asio::ip::udp::endpoint &endpoint) const;

    private:
        unsigned short _port;
        asio::io_context _ioContext;
        std::shared_ptr<asio::ip::udp::socket> _socket;
        asio::ip::udp::endpoint _endpoint;
        asio::ip::udp::endpoint _serverEndpoint;
        std::optional<ThreadPool> _threadPool;
    };

}
