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

            //SERVER
            void startAccept();
            void handleClient(std::shared_ptr<asio::ip::tcp::socket> socket);

            //CLIENT //TODO: make some of theses private
            void connect(const asio::ip::tcp::endpoint& endpoint);

            //SHARED
            void sendMessage(const std::string &message, std::shared_ptr<asio::ip::tcp::socket> socket = nullptr) const;
            void handleMessage(const std::string &message, std::shared_ptr<asio::ip::tcp::socket> socket) const;

        private:
            unsigned short _port;
            std::optional<asio::ip::tcp::acceptor> _acceptor;
            std::optional<ThreadPool> _threadPool;
            std::shared_ptr<asio::ip::tcp::socket> _socket;
            asio::io_context _ioContext;
    };

}