/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include "./Velocity.hpp"

namespace rtype::components {
    struct IA {
        std::unordered_map<float, Velocity> moves;
    };
}