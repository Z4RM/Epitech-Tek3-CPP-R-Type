/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Image entity
*/


#include "./Image.hpp"

#include <spdlog/spdlog.h>

#include "Window.hpp"
#include "RType/Components/Client/SlidingBg.hpp"
#include "RType/Components/Client/Sprite.hpp"

namespace rtype::entities {
    Image::Image(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Sprite sprite, bool
    isBackground, bool isSlide) : AEntity(entityManager) {
        sprite.texture = new sf::Texture();
        sprite.sprite = new sf::Sprite();
        const int width = static_cast<int>(sprite.size.width);
        const int height = static_cast<int>(sprite.size.height);
        const sf::IntRect rect(0, 0, width, height);
        if (width > 0 && height > 0) {
            sprite.texture->loadFromFile(sprite.path, rect);
        } else {
            sprite.texture->loadFromFile(sprite.path);
        }
        sprite.sprite->setTexture(*sprite.texture);
        sprite.sprite->setPosition({sprite.pos.x, sprite.pos.y});
        if (isBackground) {
            for (auto &entity : entityManager.getEntities()) {
                auto rwindow = componentManager.getComponent<Mode>(entity);
                if (rwindow) {
                    auto size = rwindow->mode;
                    sprite.sprite->scale( size.width / sprite.sprite->getGlobalBounds().width,  size.height / sprite.sprite->getGlobalBounds().height);
                    break;
                }
            }
        } else
            sprite.sprite->setScale(sprite.scale.x, sprite.scale.y);

        if (isSlide) {
            components::SlidingBg slide;
            componentManager.addComponent<components::SlidingBg>(_id, slide);
        }

        componentManager.addComponent<components::Sprite>(_id, sprite);
    };
}
