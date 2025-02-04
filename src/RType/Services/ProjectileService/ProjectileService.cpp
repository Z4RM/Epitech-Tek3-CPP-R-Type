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
    std::shared_ptr<components::Position> shooterPos, bool isSuperProjectile) {
        const auto projectileSpritePath = isSuperProjectile ? "assets/sprites/projectile/player-shots-charged.gif" : "assets/sprites/projectile/player-shots.gif";
        const float projectileVelX = isSuperProjectile ? 0.25 : 2.0;
        const int projectileDamage = isSuperProjectile ? 35 : 20;
        size_t projectileId = entityManager.createEntity();

        components::Velocity vel = {projectileVelX, 0.0, 0.0};
        components::Position pos = {shooterPos->x + 10.0f, shooterPos->y, shooterPos->z};
        componentManager.addComponent<components::Velocity>(projectileId, vel);
        componentManager.addComponent<components::Position>(projectileId, pos);
        componentManager.addComponent<components::Size>(projectileId, {10.0f, 10.0f});
        componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}});
        componentManager.addComponent<components::Speed>(projectileId, {250});
        componentManager.addComponent<components::Damage>(projectileId, {projectileDamage});
        componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true});
        #ifdef RTYPE_IS_CLIENT
        components::Sprite projectileSprite = {
            pos,
            {82.0f, 18.0f},
            projectileSpritePath,
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
            projectileSpritePath,
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
