/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Hitbox.hpp
*/

#pragma once
#include "./Position.hpp"
#include "Size.hpp"

namespace rtype::components {
    struct Hitbox
    {
        Position pos;
        Size size;
    };
}