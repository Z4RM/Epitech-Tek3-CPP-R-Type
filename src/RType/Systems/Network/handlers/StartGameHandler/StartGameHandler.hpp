/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Start game handler
*/

#pragma once
#include "RType/Systems/Network/handlers/ANetworkHandler.hpp"

namespace rtype::systems {
    class StartGameHandler final : public ANetworkHandler {
    public:
        StartGameHandler(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : ANetworkHandler
        (componentManager, entityManager) {};
        void handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) override;
    };
}
