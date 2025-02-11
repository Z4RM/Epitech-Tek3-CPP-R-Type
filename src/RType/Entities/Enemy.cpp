/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Enemy.cpp
*/


#include "Enemy.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/TextureManager/TextureManager.hpp"

rtype::entities::Enemy::Enemy(
    rtype::ecs::EntityManager &entityManager,
    rtype::ecs::ComponentManager &componentManager,
    components::Position pos,
    components::Velocity vel,
    components::Size size,
    components::Sprite &sprite,
    const components::Animation &animation,
    components::NetId netId,
    components::Speed speed
) {
    _id = entityManager.createEntity();
    sprite.sprite = std::make_shared<sf::Sprite>();
    sprite.texture = TextureManager::getInstance().getTexture("enemy");
    sprite.sprite->setTexture(*sprite.texture);
    sf::Vector2f spriteSize(sprite.sprite->getTextureRect().width, sprite.sprite->getTextureRect().height);
    sprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
    sprite.sprite->setPosition({pos.x, pos.y});
    sprite.sprite->setScale(2, 2);

    sf::RectangleShape hitboxRect;

    hitboxRect.setOrigin(size.width / 2, size.height / 2);
    hitboxRect.setPosition(pos.x, pos.y);
    hitboxRect.setSize({size.width, size.height});
    hitboxRect.setOutlineColor(sf::Color::Green);
    hitboxRect.setOutlineThickness(2.f);
    hitboxRect.setFillColor(sf::Color::Transparent);
    componentManager.addComponent<components::Animation>(_id, animation, entityManager);
    componentManager.addComponent<components::Position>(_id, pos, entityManager);
    componentManager.addComponent<components::Velocity>(_id, vel, entityManager);
    componentManager.addComponent<components::Size>(_id, size, entityManager);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size, hitboxRect}, entityManager);
    componentManager.addComponent<components::Speed>(_id, speed, entityManager);
    componentManager.addComponent<components::NetId>(_id, netId, entityManager);

    components::Health health(50, {sprite.pos.x, sprite.pos.y}, size);
    componentManager.addComponent<components::Health>(_id, health, entityManager);

    components::Damage damage = {200};
    componentManager.addComponent<components::Damage>(_id, damage, entityManager);


    std::vector<components::Velocity> move;
    move.emplace_back(components::Velocity({-0.5, 0, 0}));
    move.emplace_back(components::Velocity({-0.5, -0.5, 0}));
    move.emplace_back(components::Velocity({-0.5, 0.5, 0}));
    componentManager.addComponent<components::IA>(_id, {move, models::BASIC, 0.5, 1}, entityManager);
    componentManager.addComponent<components::Sprite>(_id, sprite, entityManager);
}

#else

rtype::entities::Enemy::Enemy(
    rtype::ecs::EntityManager &entityManager,
    rtype::ecs::ComponentManager &componentManager,
    components::Position pos,
    components::Velocity vel,
    components::Size size,
    components::NetId netId,
    components::Speed speed
) {
    _id = entityManager.createEntity();
    componentManager.addComponent<components::Position>(_id, pos, entityManager);
    componentManager.addComponent<components::Velocity>(_id, vel, entityManager);
    componentManager.addComponent<components::Size>(_id, size, entityManager);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size}, entityManager);
    componentManager.addComponent<components::Speed>(_id, speed, entityManager);
    componentManager.addComponent<components::NetId>(_id, netId, entityManager);

    components::Health health(50);
    componentManager.addComponent<components::Health>(_id, health, entityManager);

    components::Damage damage = {200};
    componentManager.addComponent<components::Damage>(_id, damage, entityManager);

    std::vector<components::Velocity> move;
    move.emplace_back(components::Velocity({-0.5, 0, 0}));
    move.emplace_back(components::Velocity({-0.5, -0.5, 0}));
    move.emplace_back(components::Velocity({-0.5, 0.5, 0}));
    componentManager.addComponent<components::IA>(_id, {move, models::BASIC, 0.5, 1}, entityManager);
}
#endif