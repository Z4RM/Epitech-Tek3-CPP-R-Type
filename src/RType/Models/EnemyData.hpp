/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "Components.hpp"

namespace rtype::models {

    struct EnemyData {
        components::Position pos{};
        components::Velocity vel{0, 0, 0};
        components::Size size{};
        components::NetId netId;
        int health = 0;
    };
}