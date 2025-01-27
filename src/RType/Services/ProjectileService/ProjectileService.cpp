/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Projectile Service
*/

#include "ProjectileService.hpp"
#include "Components.hpp"

namespace rtype::services {
    void ProjectileService::createProjectile(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
    std::shared_ptr<components::Position> shooterPos) {
        size_t projectileId = entityManager.createEntity();

        components::Velocity vel = {2.0, 0.0, 0.0};
        components::Position pos = {shooterPos->x + 10.0f, shooterPos->y, shooterPos->z};
        componentManager.addComponent<components::Velocity>(projectileId, vel);
        componentManager.addComponent<components::Position>(projectileId, pos);
        componentManager.addComponent<components::Size>(projectileId, {10.0f, 10.0f});
        componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}});
        componentManager.addComponent<components::Speed>(projectileId, {250});
        componentManager.addComponent<components::Damage>(projectileId, {20});
        componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true});
    }

}
