/*
** EPITECH PROJECT, 2025
** RType
** File description:
** BossMachin
*/

#pragma once

#include <RType/Components/Shared/Position.hpp>

#include "ECS/AEntity.hpp"
#include "ECS/ComponentManager.hpp"
#include "RType/Components/Shared/NetId.hpp"

namespace rtype::entities {
    class BossMachin final : ecs::AEntity {
    public:
        BossMachin(
            ecs::ComponentManager &componentManager,
            ecs::EntityManager &entityManager,
            components::Position pos,
            components::NetId netId
            );
    };
}