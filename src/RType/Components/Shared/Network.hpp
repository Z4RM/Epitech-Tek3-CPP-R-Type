/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include <asio.hpp>
namespace rtype::components {

    //TODO: endpoint should maybe be a shared_ptr
    struct NetworkConnection {
        asio::ip::udp::endpoint endpoint;
    };
}