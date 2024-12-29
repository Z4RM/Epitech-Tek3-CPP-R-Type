/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TCP Network connection
*/

#pragma once

#include <optional>
#include <RType/ThreadPool/ThreadPool.hpp>
#include "asio.hpp"

namespace rtype::network {

    class TCPNetwork {
        public:
            explicit TCPNetwork(unsigned short port = 0);
            ~TCPNetwork();

            void start();

            void startAccept();
            void handleClient(std::shared_ptr<asio::ip::tcp::socket> socket);

        private:
            unsigned short _port; ///< port of the self connection (0 for any)
            std::optional<asio::ip::tcp::acceptor> _acceptor;
            std::optional<ThreadPool> _threadPool;
            std::optional<asio::ip::tcp::socket> _serverSocket;
            asio::io_context _ioContext;
    };

}