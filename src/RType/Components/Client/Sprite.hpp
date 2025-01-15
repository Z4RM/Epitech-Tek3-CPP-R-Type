/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "../Shared/Position.hpp"
#include "../Shared/Created.hpp"
#include "../Shared/Size.hpp"
#include "../Client/ZIndex.hpp"
#include "IComponent.hpp"

namespace rtype::components {
    struct Sprite : public IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            path = value["path"];
            priority.create(value["priority"]);
            created.isCreate = true;
            texture = new sf::Texture();
            sprite = new sf::Sprite();
            rect = new sf::RectangleShape();
            size = Size();
            size.width = value["size"]["width"];
            size.height = value["size"]["height"];
            texture->loadFromFile(path);
            sprite->setTexture(*texture);
            sprite->setScale(size.width / texture->getSize().x, size.height / texture->getSize().y);
            rect->setSize({size.width, size.height});
        }
        std::string path;
        ZIndex priority;
        sf::Texture* texture;
        sf::Sprite* sprite;
        sf::RectangleShape *rect;
        Created created;
        Size size;
    };
}
