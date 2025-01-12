/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Enemy.cpp
*/


#include "Enemy.hpp"

#ifdef RTYPE_IS_CLIENT

rtype::entities::Enemy::Enemy(
    rtype::ecs::EntityManager &entityManager,
    rtype::ecs::ComponentManager &componentManager,
    components::Position pos,
    components::Velocity vel,
    components::Size size,
    components::Sprite &sprite,
    const components::Animation &animation
) {
    _id = entityManager.createEntity();
    sprite.texture = new sf::Texture();
    sprite.sprite = new sf::Sprite();
    const int width = static_cast<int>(sprite.size.width);
    const int height = static_cast<int>(sprite.size.height);
    const sf::IntRect rect(0, 0, width, height);
    sprite.texture->loadFromFile(sprite.path, rect);
    sprite.sprite->setTexture(*sprite.texture);
    sprite.sprite->setPosition({pos.x, pos.y});
    sprite.sprite->setScale(2, 2);
    componentManager.addComponent<components::Sprite>(_id, sprite);
    componentManager.addComponent<components::Animation>(_id, animation);
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});


    std::unordered_map<float, components::Velocity> move;
    move.insert({
        0,
        {-0.5, 0, 0}
    });
    componentManager.addComponent<components::IA>(_id, {move});
}

#else

rtype::entities::Enemy::Enemy(
    rtype::ecs::EntityManager &entityManager,
    rtype::ecs::ComponentManager &componentManager,
    components::Position pos,
    components::Velocity vel,
    components::Size size
) {
    _id = entityManager.createEntity();
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});
}

#endif
