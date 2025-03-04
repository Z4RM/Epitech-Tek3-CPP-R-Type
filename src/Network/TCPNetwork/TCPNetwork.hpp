/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TCP Network connection
*/

#pragma once

#include <map>
#include <optional>
#include <utility>
#include "ThreadPool/ThreadPool.hpp"
#include "asio.hpp"
#include "Packets.hpp"
#include "RType/Systems/Network/handlers/INetworkHandler.hpp"

#define BUFFER_SIZE 1024

namespace rtype::network {

    /** @brief Represent a TCP network (client or server) **/
    class TCPNetwork {
        public:
            explicit TCPNetwork(unsigned short port = 0);
            ~TCPNetwork();

            /** @brief starting the network **/
            void start();

            //SERVER

            /** @brief starting to accept new connections **/
            void startAccept();

            /**
             * @brief Start receiving async data from a client
             * @param socket the socket of the client
             * **/
            void handleClient(std::shared_ptr<asio::ip::tcp::socket> socket);

            // CLIENT //
            /**
             * @brief Connect to the given server endpoint
             * @param endpoint to endpoint to connect to
             * **/
            void connect(asio::ip::tcp::endpoint endpoint); // glibchecker-ignore

            // SHARED //

            /**
             * @brief send a new packet to the given socket
             * @param packet the packet to send
             * @param socket the socket to send to
             * **/
            void sendPacket(const IPacket &packet, std::shared_ptr<asio::ip::tcp::socket> socket = nullptr) const;

            /**
             * @brief handle a new received packet
             * @param buffer the buffer received
             * @param socket the socket that send the packet
             * **/
            void handlePacket(const std::vector<char> &buffer, std::shared_ptr<asio::ip::tcp::socket>
            socket) const;

            inline bool getStarted() const { return this->_started; };

            static TCPNetwork &getInstance(unsigned short port = 0);

            void registerOnPlayerDisconnect(std::function<void(std::shared_ptr<asio::ip::tcp::socket>)> fn) { this->_onPlayerDisconnect =
            std::move(fn); }

            inline void registerNetHandler(EPacketCode code, std::unique_ptr<systems::INetworkHandler> handler) {
                this->_netHandlers[code] = std::move(handler);
            };

            void setStop(bool state);

        private:
            unsigned short _port; ///< port of the server
            std::optional<asio::ip::tcp::acceptor> _acceptor; ///< asio acceptor for the server
            std::optional<ThreadPool> _threadPool; ///< the thread pool for multi threading tasks
            std::shared_ptr<asio::ip::tcp::socket> _socket; ///< client socket
            asio::io_context _ioContext; ///< asio context
            bool _started = false;
            std::function<void(std::shared_ptr<asio::ip::tcp::socket>)> _onPlayerDisconnect;

            std::map<EPacketCode, std::unique_ptr<systems::INetworkHandler>> _netHandlers {};
    };

}
