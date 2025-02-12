/*
** EPITECH PROJECT, 2025
** RType
** File description:
** IA.hpp
*/

#pragma once

#include "./Velocity.hpp"
#include "RType/Models/EEnemyType.hpp"
#include <vector>

namespace rtype::components {
    struct IA {
        std::vector<Velocity> moves {};
        models::EEnemyType type = models::BASIC;
        float minMoveTime = 0;
        float maxMoveTime = 0;
        int lastMoveIndex = -1;
        float timeUntilNextMove = 0.0f;
    };
}
