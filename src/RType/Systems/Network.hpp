/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TODO: add description
*/

#pragma once
#include <list>

#include "ECS.hpp"
#include "Network/UDPNetwork/UDPNetwork.hpp"
#include "RType/ModeManager/ModeManager.hpp"
namespace rtype::systems {
    class Network {
    public:
        static int playerId;

        static void udpProcess(ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);
        static void addUdpHandlers(network::UDPNetwork &network, ecs::EntityManager& entityManager, ecs::ComponentManager&
        componentManager);

        static void schedulePacketSending(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
        network::UDPNetwork &network, std::shared_ptr<asio::steady_timer> timer);


        static void tcpProcess(ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);
    };
}