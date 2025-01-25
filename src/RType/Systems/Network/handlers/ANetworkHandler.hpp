/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Abstract Network handler class
*/

#pragma once
#include "INetworkHandler.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/EntityManager.hpp"

namespace rtype::systems {
    class ANetworkHandler : public virtual INetworkHandler {
    public:
        virtual void handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) override {};
        virtual void handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) override {};
    protected:
        ANetworkHandler(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : _componentManager
        (componentManager), _entityManager(entityManager) {};

        ecs::ComponentManager &_componentManager;
        ecs::EntityManager &_entityManager;
    };
}
