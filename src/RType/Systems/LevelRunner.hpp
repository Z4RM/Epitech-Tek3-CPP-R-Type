/*
** EPITECH PROJECT, 2025
** RType
** File description:
** LevelRunner
*/

#pragma once
#include "ECS.hpp"

namespace rtype::systems {
    class LevelRunner {
    public:
        static void process(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);
    };
}