/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player shoot handler
*/

#pragma once
#include "RType/Systems/Network/handlers/ANetworkHandler.hpp"


namespace rtype::systems {
    class PlayerShootHandler final : public ANetworkHandler {
    public:
        PlayerShootHandler(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : ANetworkHandler
        (componentManager, entityManager) {};
        void handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) override;
    };
}
