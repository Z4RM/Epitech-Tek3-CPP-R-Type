/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Interface for handlers
*/

#pragma once
#include "Network/Packets/IPacket.hpp"

namespace rtype::systems {
    class INetworkHandler {
    public:
        virtual ~INetworkHandler() = default;
        virtual void handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) = 0;
        virtual void handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) = 0;
    };
}
