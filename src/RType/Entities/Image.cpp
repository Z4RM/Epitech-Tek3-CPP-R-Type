/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Image entity
*/


#include "./Image.hpp"

#include "RType/Components/Client/Sprite.hpp"

namespace rtype::entities {
    Image::Image(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Sprite sprite) :
    AEntity(entityManager) {
        this->_id = entityManager.createEntity();

        sprite.texture = new sf::Texture();
        sprite.sprite = new sf::Sprite();
        const int width = static_cast<int>(sprite.size.width);
        const int height = static_cast<int>(sprite.size.height);
        const sf::IntRect rect(0, 0, width, height);
        sprite.texture->loadFromFile(sprite.path, rect);
        sprite.sprite->setTexture(*sprite.texture);
        sprite.sprite->setPosition({sprite.pos.x, sprite.pos.y});

        componentManager.addComponent<components::Sprite>(_id, sprite);
    };
}
