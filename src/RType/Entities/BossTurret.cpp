/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Boss turret
*/

#include "BossTurret.hpp"

#include <spdlog/spdlog.h>

#include "Components.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/TextureManager/TextureManager.hpp"
namespace rtype::entities {
    BossTurret::BossTurret(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Position
    pos, components::NetId netId) : AEntity(entityManager) {
        _id = entityManager.createEntity();
        components::Sprite sprite;
        sprite.priority = { 1 };
        components::Size size = {592 / 2, 176 / 2};

        sprite.sprite = std::make_shared<sf::Sprite>();
        sprite.texture = TextureManager::getInstance().getTexture("boss_turret");
        sprite.sprite->setTexture(*sprite.texture);
        sf::Vector2f spriteSize(sprite.sprite->getTextureRect().width, sprite.sprite->getTextureRect().height);
        sprite.sprite->setScale(0.5, 0.5);
        sprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
        sprite.sprite->setPosition({pos.x, pos.y});

        sf::RectangleShape hitboxRect;

        hitboxRect.setOrigin(size.width / 2, size.height / 2);
        hitboxRect.setPosition(pos.x, pos.y);
        hitboxRect.setSize({size.width, size.height});
        hitboxRect.setOutlineColor(sf::Color::Green);
        hitboxRect.setOutlineThickness(2.f);
        hitboxRect.setFillColor(sf::Color::Transparent);
        componentManager.addComponent<components::Position>(_id, pos, entityManager);
        componentManager.addComponent<components::Size>(_id, size, entityManager);
        componentManager.addComponent<components::Hitbox>(_id, {pos, size, hitboxRect}, entityManager);
        componentManager.addComponent<components::NetId>(_id, netId, entityManager);
        componentManager.addComponent<components::Velocity>(_id, {0, 0}, entityManager);
        componentManager.addComponent<components::Size>(_id, {592, 176}, entityManager);
        componentManager.addComponent<components::Speed>(_id, {1}, entityManager);

        components::Health health(2000, {sprite.pos.x, sprite.pos.y}, size);
        componentManager.addComponent<components::Health>(_id, health, entityManager);

        components::Damage damage = {200};
        componentManager.addComponent<components::Damage>(_id, damage, entityManager);


        std::unordered_map<float, components::Velocity> move;
        move.insert({
            0,
            {0, 0, 0}
        });
        componentManager.addComponent<components::IA>(_id, {move, models::BOSS_TURRET}, entityManager);
        componentManager.addComponent<components::Sprite>(_id, sprite, entityManager);
    }
}
#else
namespace rtype::entities {
    BossTurret::BossTurret(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Position
    pos, components::NetId netId)
     : AEntity
    (entityManager) {
        componentManager.addComponent<components::Position>(_id, pos, entityManager);
        componentManager.addComponent<components::Hitbox>(_id, {pos, 592 / 2, 176 / 2}, entityManager);
        componentManager.addComponent<components::NetId>(_id, netId, entityManager);
        componentManager.addComponent<components::Velocity>(_id, {0, 0}, entityManager);
        componentManager.addComponent<components::Size>(_id, {592 / 2, 176 / 2}, entityManager);
        componentManager.addComponent<components::Speed>(_id, {0}, entityManager);

        components::Health health(2000);
        componentManager.addComponent<components::Health>(_id, health, entityManager);

        components::Damage damage = {200};
        componentManager.addComponent<components::Damage>(_id, damage, entityManager);

        std::unordered_map<float, components::Velocity> move;
        move.insert({
            0,
            {0, 0, 0}
        });
        componentManager.addComponent<components::IA>(_id, {move, models::BOSS_TURRET}, entityManager);
    }
}
#endif