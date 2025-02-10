/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Enemy.cpp
*/


#include "Enemy.hpp"

#include "RType/TextureManager/TextureManager.hpp"

#ifdef RTYPE_IS_CLIENT

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
    sprite.sprite->setPosition({pos.x, pos.y});
    sprite.sprite->setScale(2, 2);
    componentManager.addComponent<components::Animation>(_id, animation);
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});
    componentManager.addComponent<components::Speed>(_id, speed);
    componentManager.addComponent<components::NetId>(_id, netId);

    components::Health health(50, {sprite.pos.x, sprite.pos.y}, size);
    componentManager.addComponent<components::Health>(_id, health);

    components::Damage damage = {200};
    componentManager.addComponent<components::Damage>(_id, damage);


    std::unordered_map<float, components::Velocity> move;
    move.insert({
        0,
        {-0.5, 0, 0}
    });
    componentManager.addComponent<components::IA>(_id, {move});
    componentManager.addComponent<components::Sprite>(_id, sprite);
}

#endif

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
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});
    componentManager.addComponent<components::Speed>(_id, speed);
    componentManager.addComponent<components::NetId>(_id, netId);

    components::Health health(50);
    componentManager.addComponent<components::Health>(_id, health);

    components::Damage damage = {200};
    componentManager.addComponent<components::Damage>(_id, damage);

    std::unordered_map<float, components::Velocity> move;
    move.insert({
        0,
        {-0.5, 0, 0}
    });
    componentManager.addComponent<components::IA>(_id, {move});
}
