/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDPServer.hpp
*/

#ifndef RTYPE_UDPSERVER_HPP_
#define RTYPE_UDPSERVER_HPP_

#include <optional>
#include <queue>
#include "asio.hpp"

// TODO: documentation
#define BUFFER_SIZE 1024

namespace rtype::server::network {


    struct ClientInfo {
        std::string name;
    };

    // TODO: documentation
    class UDPServer {

    public:
        // TODO: documentation
        class StartException : public std::exception {
        public:
            // TODO: documentation
            StartException();
        };

        static UDPServer &getInstance(ushort port = 4242);

        static void initialize(ushort port) { getInstance(port); };

        // TODO: documentation
        UDPServer(ushort port);

        // TODO: documentation
        /**
         * @throw StartException If the server is already started.
         */
        void start();

        // TODO: documentation
        void stop();

    private:
        // TODO: refactor
        void _receive();

        // TODO: refactor
        void _send();

        // TODO: documentation
        ushort _port;

        void _connect(std::string &ip, ushort port);

        // TODO: documentation
        std::atomic<bool> _running = false;

        // TODO: documentation
        asio::io_context _ioContext;

        // TODO: documentation
        std::optional<asio::ip::udp::socket> _socket;

        // TODO: documentation
        std::optional<asio::ip::udp::endpoint> _endpoint;

        // TODO: documentation
        std::thread _thread;

        // TODO: documentation
        std::array<char, BUFFER_SIZE> _buffer;

        std::queue<std::string> _packetsToHandle{};


        std::unordered_map<asio::ip::udp::endpoint, ClientInfo> _clients;

    };
}

#endif /* !RTYPE_UDPSERVER_HPP_ */
