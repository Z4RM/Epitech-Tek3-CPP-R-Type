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
        components::Health health(1);
        componentManager.addComponent<components::Velocity>(projectileId, vel);
        componentManager.addComponent<components::Position>(projectileId, pos);
        componentManager.addComponent<components::Size>(projectileId, {10.0f, 10.0f});
        componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}});
        componentManager.addComponent<components::Speed>(projectileId, {250});
        componentManager.addComponent<components::Damage>(projectileId, {20});
        componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true});
        //componentManager.addComponent<components::Health>(projectileId, health);
        #ifdef RTYPE_IS_CLIENT
        components::Sprite projectileSprite = {
            pos,
            {82.0f, 18.0f},
            "assets/sprites/projectile/player-shots.gif",
            {1},
            {1.0, 1.0},
            std::make_shared<sf::Texture>(),
            std::make_shared<sf::Sprite>()
        };
        projectileSprite.texture->loadFromFile(projectileSprite.path);
        projectileSprite.sprite->setTexture(*projectileSprite.texture);
        sf::IntRect textureRect(82, 165, 82, 18);
        projectileSprite.sprite->setTextureRect(textureRect);
        projectileSprite.sprite->setPosition({pos.x, pos.y});
        components::Animation projAnim = {
            "assets/sprite/projectile/player-shots.gif",
            2,
            10
        };
        componentManager.addComponent<components::Animation>(projectileId, projAnim);
        componentManager.addComponent<components::Sprite>(projectileId, projectileSprite);
        components::Projectile projectile = {
            pos,
            vel,
            projAnim,
            projectileSprite
        };
        componentManager.addComponent<components::Projectile>(projectileId, projectile);
        #endif
    }
}
