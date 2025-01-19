/*
** EPITECH PROJECT, 2025
** RType
** File description:
** component projectile
*/

#pragma once
#include "Position.hpp"
#include "Velocity.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/Components/Client/Sprite.hpp"
#include "../Client/Animation.hpp"
#endif

namespace rtype::components {
    struct Projectile {
        Position position{};
        Velocity velocity{};
    #ifdef RTYPE_IS_CLIENT
        Animation animation;
        Sprite sprite;
        sf::Clock animationClock;
    #endif
    };
}
