/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Sprite.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "../Shared/Position.hpp"
#include "../Shared/Created.hpp"
#include "../Shared/Size.hpp"
#include "../Client/ZIndex.hpp"

namespace rtype::components {
    struct Sprite
    {
        Position pos;
        Size size;
        std::string path;
        ZIndex priority;
        sf::Vector2f scale = {1, 1};
        std::shared_ptr<sf::Texture> texture;
        std::shared_ptr<sf::Sprite> sprite;
        Created created;
    };
}