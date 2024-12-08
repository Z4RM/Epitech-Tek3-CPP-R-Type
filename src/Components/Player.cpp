/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#include "Player.hpp"

#include <utility>

Player::Player(
    rtype::ecs::EntityManager& entityManager,
    rtype::ecs::ComponentManager& componentManager,
    const Position pos,
    const Velocity vel,
    const Size size,
    const Sprite& sprite,
    const Animation& animation)
{
    _id = entityManager.createEntity();
    componentManager.addComponent<Position>(_id, pos);
    componentManager.addComponent<Velocity>(_id, vel);
    componentManager.addComponent<Size>(_id, size);
    componentManager.addComponent<Hitbox>(_id, {pos, size});
    componentManager.addComponent<Sprite>(_id, sprite);
    componentManager.addComponent<Animation>(_id, animation);
}
