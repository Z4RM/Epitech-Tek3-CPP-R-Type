/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#include "Player.hpp"

#include <utility>

rtype::components::Player::Player(
    rtype::ecs::EntityManager& entityManager,
    rtype::ecs::ComponentManager& componentManager,
    const Position pos,
    const Velocity vel,
    const Size size,
    Sprite& sprite,
    const Animation& animation)
{
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
    componentManager.addComponent<Position>(_id, pos);
    componentManager.addComponent<Velocity>(_id, vel);
    componentManager.addComponent<Size>(_id, size);
    componentManager.addComponent<Hitbox>(_id, {pos, size});
    componentManager.addComponent<Sprite>(_id, sprite);
    componentManager.addComponent<Animation>(_id, animation);
}
