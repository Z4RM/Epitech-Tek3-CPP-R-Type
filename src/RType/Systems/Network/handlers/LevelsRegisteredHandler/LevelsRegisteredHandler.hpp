/*
** EPITECH PROJECT, 2025
** RType
** File description:
** LevelsRegistered handler
*/

#pragma once
#include "RType/Systems/Network/handlers/ANetworkHandler.hpp"

namespace rtype::systems {
    class LevelsRegisteredHandler final : public ANetworkHandler {
    public:
        LevelsRegisteredHandler(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : ANetworkHandler(componentManager, entityManager) {};
        void handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) override;
    };
}