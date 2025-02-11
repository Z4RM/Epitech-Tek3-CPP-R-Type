/*
** EPITECH PROJECT, 2025
** RType
** File description:
** IA.hpp
*/

#pragma once

#include "./Velocity.hpp"
#include "RType/Models/EEnemyType.hpp"

namespace rtype::components {
    struct IA {
        std::unordered_map<float, Velocity> moves;
        models::EEnemyType type = models::BASIC;
    };
}
