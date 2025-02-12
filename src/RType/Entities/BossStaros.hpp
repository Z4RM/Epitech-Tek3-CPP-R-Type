/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Boss staros
*/

#pragma once

#include <RType/Components/Shared/Position.hpp>

#include "ECS/AEntity.hpp"
#include "ECS/ComponentManager.hpp"
#include "RType/Components/Shared/NetId.hpp"

namespace rtype::entities {
    class BossStaros final : ecs::AEntity {
    public:
        BossStaros(
            ecs::ComponentManager &componentManager,
            ecs::EntityManager &entityManager,
            components::Position pos,
            components::NetId netId
            );
    };
}