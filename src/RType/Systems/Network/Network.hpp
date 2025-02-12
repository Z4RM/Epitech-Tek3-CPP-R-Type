/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Network.hpp
*/

#pragma once
#include <list>

#include "ECS.hpp"
#include "Network/UDPNetwork/UDPNetwork.hpp"
#include "RType/ModeManager/ModeManager.hpp"

#define PLAYER_SPEED 100

namespace rtype::systems {
    /**
     * @brief Network systems class
     * **/
    class Network {
    public:
        static std::atomic<int> globalNetId;

        /** @brief Process the udp system
        * @param entityManager the ecs entity manager
        * @param componentManager the ecs component manager
        * **/
        static void udpProcess(ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);


        /** @brief sending continually packets of the game data
        * @param entityManager the entity manager
        * @param componentManager the componentManager
        * @param network the udp network
        * @param timer the clock timer
        **/
        static void schedulePacketSending(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
        network::UDPNetwork &network, std::shared_ptr<asio::steady_timer> timer);


        /** @brief Process the tcp system
        * @param entityManager the ecs entity manager
        * @param componentManager the ecs component manager
        * **/
        static void tcpProcess(ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);
    };
}