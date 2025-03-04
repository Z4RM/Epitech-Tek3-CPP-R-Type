/*
** EPITECH PROJECT, 2025
** RType
** File description:
** EnemyData.hpp
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
        EEnemyType type = EEnemyType::BASIC;
    };
}