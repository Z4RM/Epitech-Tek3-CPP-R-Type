/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player service
*/

#pragma once
#include "RType/Entities/Player.hpp"

namespace rtype::services {
    class PlayerService {
    public:
        static void createPlayer(int netId, bool actualPlayer, ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);
    };
}