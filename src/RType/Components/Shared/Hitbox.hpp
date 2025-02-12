/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Hitbox.hpp
*/

#pragma once

#include "./Position.hpp"
#include "Size.hpp"

#ifdef RTYPE_IS_CLIENT
#include <SFML/Graphics/RectangleShape.hpp>
#endif

namespace rtype::components {
#ifdef RTYPE_IS_CLIENT
    struct Hitbox
    {
        Position pos; //Why it's needed ? the entity should already have a position no ?
        Size size;
        sf::RectangleShape rect;
    };
#else
    struct Hitbox
    {
        Position pos; //Why it's needed ? the entity should already have a position no ?
        Size size;
    };
#endif

}