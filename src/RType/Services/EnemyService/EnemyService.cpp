/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Enemy service
*/

#include "EnemyService.hpp"

#include <spdlog/spdlog.h>

#include "RType/Entities/BossTurret.hpp"
#include "RType/Systems/Network/Network.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/TextureManager/TextureManager.hpp"
#endif

namespace rtype::services {
    void EnemyService::createEnemy(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
    components::Position pos, int netId, models::EEnemyType type) {
#ifdef RTYPE_IS_CLIENT
        if (type == models::BOSS_TURRET) {
            entities::BossTurret boss(componentManager, entityManager, pos, {netId});
            return;
        }
        if (type == models::TURRET) {
            EnemyService::createTurret(entityManager, componentManager, pos, netId);
            return;
        }
        components::Sprite sprite3 = {{600, 100, 0}, {33, 36}, "assets/sprites/enemy.gif", {1}};
        rtype::entities::Enemy enemy(
            entityManager,
            componentManager,
            pos,
            {0, 0, 0},
            {64, 64},
            sprite3,
            {"", 0, 0},
            { netId }
        );
#else
        systems::Network::globalNetId.store(systems::Network::globalNetId.load() + 1);

        if (type == models::BOSS_TURRET) {
            entities::BossTurret boss(componentManager, entityManager, pos, {systems::Network::globalNetId.load()});
            return;
        }
        rtype::entities::Enemy enemy(
        entityManager,
        componentManager,
        pos,
        {0, 0, 0},
        {64, 64},
        {systems::Network::globalNetId.load()}
        );

#endif
    }

    unsigned int EnemyService::createTurret(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
    components::Position pos, int netId) {
        unsigned int turretEntity = entityManager.createEntity();

        components::Health health(200);

        componentManager.addComponent<components::Position>(turretEntity, pos, entityManager);
        componentManager.addComponent<components::Health>(turretEntity, health, entityManager);
        componentManager.addComponent<components::Velocity>(turretEntity, {0, 0}, entityManager);
        componentManager.addComponent<components::Size>(turretEntity, {32 * 1.2, 15 * 1.2}, entityManager);
        componentManager.addComponent<components::Speed>(turretEntity, { 0 }, entityManager);
        componentManager.addComponent<components::NetId>(turretEntity, { netId }, entityManager);
        std::vector<components::Velocity> move = {};
        componentManager.addComponent<components::IA>(turretEntity, {move, models::TURRET, 0, 0}, entityManager);

#ifdef RTYPE_IS_CLIENT
        components::Sprite sprite;
        sprite.priority = { 2 };
        sprite.sprite = std::make_shared<sf::Sprite>();
        sprite.texture = TextureManager::getInstance().getTexture("turret");
        sprite.sprite->setTexture(*sprite.texture);
        sf::Vector2f spriteSize(sprite.sprite->getTextureRect().width, sprite.sprite->getTextureRect().height);
        sprite.sprite->setScale(1.5, 1.5);
        sprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
        sf::RectangleShape hitboxRect;

        hitboxRect.setOrigin(32 * 1.2 / 2, 15 * 1.2 / 2);
        hitboxRect.setPosition(pos.x, pos.y);
        hitboxRect.setSize({32 * 1.2, 15 * 1.2});
        hitboxRect.setOutlineColor(sf::Color::Green);
        hitboxRect.setOutlineThickness(2.f);
        hitboxRect.setFillColor(sf::Color::Transparent);
        componentManager.addComponent<components::Hitbox>(turretEntity, {pos, {32 * 1.2, 15 * 1.2}, hitboxRect}, entityManager);
        componentManager.addComponent<components::Sprite>(turretEntity, sprite, entityManager);
#else
        componentManager.addComponent<components::Hitbox>(turretEntity, {pos, 32 * 1.2, 15 * 1.2}, entityManager);
#endif
        return turretEntity;
    }


}
