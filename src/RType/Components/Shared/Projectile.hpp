/*
** EPITECH PROJECT, 2025
** RType
** File description:
** component projectile
*/

#pragma once
#include "Position.hpp"
#include "Velocity.hpp"
#include "RType/Components/Client/Sprite.hpp"

#ifdef RTYPE_IS_CLIENT
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
