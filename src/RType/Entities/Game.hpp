/*
** EPITECH PROJECT, 2025
** RType
** File description:
** game entity
*/

#pragma once
#include "ECS/AEntity.hpp"

namespace rtype::entities {
    class Game final : ecs::AEntity {
    public:
        Game(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager);
    };
}
