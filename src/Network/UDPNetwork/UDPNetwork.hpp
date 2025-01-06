/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDPServer.hpp
*/

#pragma once

#include <optional>
#include <ThreadPool/ThreadPool.hpp>
#include "asio.hpp"
#include "Packets.hpp"

#define BUFFER_SIZE 1024

namespace rtype::network {

    /** @brief Represent a UDP Network (client or server) **/
    class UDPNetwork {
    public:
        explicit UDPNetwork(unsigned short port = 0);
        ~UDPNetwork();

        /** @brief Start the network **/
        void start();

        // SERVER //
        /**
         * @brief handle a received packet
         * @param endpoint endpoint that sent the packet
         * @param buffer buffer containing the packet
         * **/
        void handlePacket(const asio::ip::udp::endpoint &endpoint, const std::vector<char> &buffer);

        // CLIENT //
        /**
        * @brief send a packet to the given endpoint
        * @param packet the packet to send
        * @param endpoint the endpoint to send the packet
        * **/
        void sendPacket(const IPacket &packet, const asio::ip::udp::endpoint &endpoint) const;

        // SHARED //
        /** @brief starting receiving packets **/
        void startReceive();

        inline bool getStarted() const { return this->_started; };

    private:
        unsigned short _port; ///< port of the server
        asio::io_context _ioContext; ///< asio context
        std::shared_ptr<asio::ip::udp::socket> _socket; ///< socket of the network
        asio::ip::udp::endpoint _endpoint; ///< endpoint that stock the new received packet's endpoint
        asio::ip::udp::endpoint _serverEndpoint; ///< endpoint of the server
        std::optional<ThreadPool> _threadPool; ///< thread pool
        bool _started = false;
    };

}
