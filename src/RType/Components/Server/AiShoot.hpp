/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Ai shoot
*/

#pragma once
#include <chrono>

namespace rtype::components {
    struct AiShoot {
        float cooldown = 0.0f;
        std::chrono::time_point<std::chrono::steady_clock> clock = std::chrono::steady_clock::now();
    };
}
