/*
** EPITECH PROJECT, 2025
** RType
** File description:
** monster spawner
*/

#pragma once

#include "ECS.hpp"

namespace rtype::systems {
    class MonsterSpawner {
    public:
        static void spawnMonster(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);
    private:
        static std::chrono::steady_clock::time_point _lastSpawnTime;
    };
}