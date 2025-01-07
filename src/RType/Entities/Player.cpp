/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#include "Player.hpp"

#include <iostream>

#include "RType/ModeManager/ModeManager.hpp"

#ifdef RTYPE_IS_CLIENT

rtype::entities::Player::Player(
        rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager,
        const components::Position pos,
        const components::Velocity vel,
        const components::Size size,
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

    size_t id = _id;

    // Press
    _inputs.keyActions.insert({
        sf::Keyboard::Key::Z,
        {sf::Event::KeyPressed, [id, pos, &componentManager]() {
            auto *vel = componentManager.getComponent<components::Velocity>(id);
            if (vel != nullptr) {
                vel->y = -1;
            }
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::S,
        {sf::Event::KeyPressed, [id, &componentManager]() {
            componentManager.getComponent<components::Velocity>(id)->y = 1;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::Q,
        {sf::Event::KeyPressed, [id, &componentManager]() {
            componentManager.getComponent<components::Velocity>(id)->x = -1;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::D,
        {sf::Event::KeyPressed, [id, &componentManager]() {
            componentManager.getComponent<components::Velocity>(id)->x = 1;
        }}
    });

    // Release
    _inputs.keyActions.insert({
        sf::Keyboard::Key::Z,
        {sf::Event::KeyReleased, [id, pos, &componentManager]() {
            auto *vel = componentManager.getComponent<components::Velocity>(id);
            if (vel != nullptr) {
                vel->y = 0;
            }
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::S,
        {sf::Event::KeyReleased, [id, &componentManager]() {
            componentManager.getComponent<components::Velocity>(id)->y = 0;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::Q,
        {sf::Event::KeyReleased, [id, &componentManager]() {
            componentManager.getComponent<components::Velocity>(id)->x = 0;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::D,
        {sf::Event::KeyReleased, [id, &componentManager]() {
            componentManager.getComponent<components::Velocity>(id)->x = 0;
        }}
    });
    componentManager.addComponent<components::InputHandler>(id, _inputs);
}

#else

rtype::entities::Player::Player(
        rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager,
        const components::Position pos,
        const components::Velocity vel,
        const components::Size size
) {
    _id = entityManager.createEntity();
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});
}

#endif
