/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Boss turret
*/

#pragma once
#include <RType/Components/Shared/Position.hpp>

#include "ECS/AEntity.hpp"
#include "ECS/ComponentManager.hpp"
#include "RType/Components/Shared/NetId.hpp"

namespace rtype::entities {
    class BossTurret final : ecs::AEntity {
    public:
        BossTurret(
            ecs::ComponentManager &componentManager,
            ecs::EntityManager &entityManager,
            components::Position pos,
            components::NetId netId
            );

#ifdef RTYPE_IS_SERVER
        static void createTurret(unsigned int parentEntity, ecs::ComponentManager &componentManager, ecs::EntityManager
         &entityManager, components::Position turretPos, float cooldown = 0.5f);
#endif
    };
}
