/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "ECS.hpp"

namespace rtype::systems {
    class Network {
    public:
        static void udpProcess(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);
        static void tcpProcess(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);
    };
}