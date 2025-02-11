/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Image entity
*/

#include "Components.hpp"
#include "Window.hpp"
#include "Image.hpp"

namespace rtype::entities {
    Image::Image(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Sprite sprite, bool
    isBackground, bool isSlide) : AEntity(entityManager) {
        sprite.texture = std::make_shared<sf::Texture>();
        sprite.sprite = std::make_shared<sf::Sprite>();
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
            componentManager.addComponent<components::SlidingBg>(_id, slide, entityManager);
        }

        componentManager.addComponent<components::Sprite>(_id, sprite, entityManager);
    };
}
