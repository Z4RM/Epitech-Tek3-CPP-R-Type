/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Projectile Service
*/

#include "ProjectileService.hpp"
#include "Components.hpp"
#include "RType/Components/Shared/ProjectileInfo.hpp"
#include "RType/TextureManager/TextureManager.hpp"

namespace rtype::services {
    void ProjectileService::createProjectile(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
    std::shared_ptr<components::Position> shooterPos, bool isSuperProjectile, components::EventId eventId) {
        const auto projectileSpritePath = isSuperProjectile ? "assets/sprites/projectile/player-shots-charged.gif" : "assets/sprites/projectile/player-shots.gif";
        std::string projectileTextureKey = isSuperProjectile ? "super_projectile" : "projectile";
        const float projectileVelX = isSuperProjectile ? 0.25 : 2.0;
        const int projectileDamage = isSuperProjectile ? 35 : 20;
        size_t projectileId = entityManager.createEntity();

        components::Velocity vel = {projectileVelX, 0.0, 0.0};
        components::Position pos = {shooterPos->x + 10.0f, shooterPos->y, shooterPos->z};
        componentManager.addComponent<components::Velocity>(projectileId, vel, entityManager);
        componentManager.addComponent<components::Position>(projectileId, pos, entityManager);
        componentManager.addComponent<components::Size>(projectileId, {82.0f, 18.0f}, entityManager);
        componentManager.addComponent<components::Speed>(projectileId, {250}, entityManager);
        componentManager.addComponent<components::Damage>(projectileId, {projectileDamage}, entityManager);
        componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true}, entityManager);
        componentManager.addComponent<components::EventId>(projectileId, eventId, entityManager);
        componentManager.addComponent<components::ProjectileInfo>(projectileId, {isSuperProjectile}, entityManager);
        #ifdef RTYPE_IS_CLIENT
        sf::RectangleShape hitboxRect;

        hitboxRect.setPosition(pos.x, pos.y);
        hitboxRect.setOrigin(82.f / 2, 18.f / 2);
        hitboxRect.setSize({82.0f, 18.0f});
        hitboxRect.setOutlineColor(sf::Color::Green);
        hitboxRect.setOutlineThickness(2.f);
        hitboxRect.setFillColor(sf::Color::Transparent);
        componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}, hitboxRect}, entityManager);
        components::Sprite projectileSprite = {
            pos,
            {82.0f, 18.0f},
            projectileSpritePath,
            {1},
            {1.0, 1.0},
            std::make_shared<sf::Texture>(),
            std::make_shared<sf::Sprite>()
        };
        projectileSprite.texture = TextureManager::getInstance().getTexture(projectileTextureKey);
        projectileSprite.sprite->setTexture(*projectileSprite.texture, false);
        sf::IntRect textureRect(82, 165, 82, 18);
        projectileSprite.sprite->setTextureRect(textureRect);
        sf::Vector2f spriteSize(82.f, 18.f);
        projectileSprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
        projectileSprite.sprite->setPosition({pos.x, pos.y});
        components::Animation projAnim = {
            projectileSpritePath,
            2,
            10
        };
        componentManager.addComponent<components::Animation>(projectileId, projAnim, entityManager);
        componentManager.addComponent<components::Sprite>(projectileId, projectileSprite, entityManager);
        components::Projectile projectile = {
            pos,
            vel,
            projAnim,
            projectileSprite
        };
        componentManager.addComponent<components::Projectile>(projectileId, projectile, entityManager);
        #else
        componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}}, entityManager);
        #endif
    }
}
