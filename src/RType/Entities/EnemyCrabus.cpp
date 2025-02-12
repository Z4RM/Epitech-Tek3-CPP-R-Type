/*
** EPITECH PROJECT, 2025
** RType
** File description:
** enemy crabus
*/

#include "EnemyCrabus.hpp"

#include "Components.hpp"
#ifdef RTYPE_IS_CLIENT
#include "RType/TextureManager/TextureManager.hpp"
#else
#include "RType/Components/Server/AiShoot.hpp"
#endif

namespace rtype::entities {
    EnemyCrabus::EnemyCrabus(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Position
    pos, components::NetId netId) : AEntity(entityManager){
        components::Size size = {29, 28};
        componentManager.addComponent<components::Position>(_id, pos, entityManager);
#ifdef RTYPE_IS_SERVER
        componentManager.addComponent<components::Hitbox>(_id, {pos, size.width, size.height}, entityManager);
#endif
        componentManager.addComponent<components::NetId>(_id, netId, entityManager);
        componentManager.addComponent<components::Velocity>(_id, {0, 0}, entityManager);
        componentManager.addComponent<components::Size>(_id, size, entityManager);
        componentManager.addComponent<components::Speed>(_id, {200 }, entityManager);

        components::Health health(2000);
        componentManager.addComponent<components::Health>(_id, health, entityManager);

        components::Damage damage = {200};
        componentManager.addComponent<components::Damage>(_id, damage, entityManager);

        std::vector<components::Velocity> move;
        move.emplace_back(components::Velocity({0, 0.5, 0}));
        move.emplace_back(components::Velocity({0, -0.5, 0}));
        move.emplace_back(components::Velocity({0.3, -0.5, 0}));
        move.emplace_back(components::Velocity({-0.3, 0.5, 0}));
        componentManager.addComponent<components::IA>(_id, {move, models::CRABUS, 0.5, 1}, entityManager);
#ifdef RTYPE_IS_CLIENT
        components::Sprite sprite;
        sprite.priority = { 1 };

        sprite.sprite = std::make_shared<sf::Sprite>();
        sprite.texture = TextureManager::getInstance().getTexture("crabus");
        sprite.sprite->setTexture(*sprite.texture);
        sf::Vector2f spriteSize(sprite.sprite->getTextureRect().width, sprite.sprite->getTextureRect().height);
        sprite.sprite->setScale(1, 1);
        sprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
        sprite.sprite->setPosition({pos.x, pos.y});

        sf::RectangleShape hitboxRect;

        hitboxRect.setOrigin(size.width / 2, size.height / 2);
        hitboxRect.setPosition(pos.x, pos.y);
        hitboxRect.setSize({size.width, size.height});
        hitboxRect.setOutlineColor(sf::Color::Green);
        hitboxRect.setOutlineThickness(2.f);
        hitboxRect.setFillColor(sf::Color::Transparent);
        componentManager.addComponent<components::Hitbox>(_id, {pos, size, hitboxRect}, entityManager);
        componentManager.addComponent<components::Sprite>(_id, sprite, entityManager);
#else
        componentManager.addComponent<components::AiShoot>(_id, {0.3f}, entityManager);
#endif
    }

}