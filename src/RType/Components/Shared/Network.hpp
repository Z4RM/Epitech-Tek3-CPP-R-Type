/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Network.hpp
*/

#pragma once
#include <asio.hpp>
#include <optional>
namespace rtype::components {

    //TODO: endpoint should maybe be a shared_ptr
    struct NetworkConnection {
        std::shared_ptr<asio::ip::tcp::socket> socket;
        std::optional<asio::ip::udp::endpoint> endpoint;
    };
}