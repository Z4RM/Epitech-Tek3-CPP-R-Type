/*
** EPITECH PROJECT, 2025
** RType
** File description:
** EnemyServices
*/

#pragma once

#include "ECS.hpp"
#include "RType/Entities/Enemy.hpp"

namespace rtype::services {
    class EnemyService {
    public:
        static void createEnemy(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
        components::Position pos, int netId = 0, models::EEnemyType type = models::BASIC);
    };
}