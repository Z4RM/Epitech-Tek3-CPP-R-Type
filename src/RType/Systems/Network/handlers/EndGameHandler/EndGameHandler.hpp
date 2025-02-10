/*
** EPITECH PROJECT, 2025
** RType
** File description:
** EndGameHandler.hpp
*/


#pragma once
#include "RType/Systems/Network/handlers/ANetworkHandler.hpp"

namespace rtype::systems {
    class EndGameHandler final : public ANetworkHandler {
    public:
        EndGameHandler(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : ANetworkHandler(componentManager, entityManager) {};
        void handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) override;
    };
}