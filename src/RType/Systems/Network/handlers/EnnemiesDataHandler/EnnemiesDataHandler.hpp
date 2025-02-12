/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Ennemies data handler
*/

#pragma once
#include "RType/Systems/Network/handlers/ANetworkHandler.hpp"

namespace rtype::systems {
    class EnnemiesDataHandler final : public ANetworkHandler {
    public:
        EnnemiesDataHandler(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : ANetworkHandler(componentManager,
        entityManager) {};
        void handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) override;
    };
}
