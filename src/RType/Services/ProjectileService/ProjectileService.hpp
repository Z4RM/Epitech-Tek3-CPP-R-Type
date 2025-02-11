/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Projectile Service
*/

#pragma once
#include <RType/Components/Shared/Position.hpp>

#include "ECS/ComponentManager.hpp"
#include "ECS/EntityManager.hpp"

namespace rtype::services {
    class ProjectileService {
    public:
        static void createProjectile(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
        std::shared_ptr<components::Position> shooterPos, bool isSuperProjectile);
    };
}
