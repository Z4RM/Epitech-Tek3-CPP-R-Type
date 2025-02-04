/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Spawn
*/

#pragma once

#include <vector>
#include "./EEnemyType.hpp"

namespace rtype::models {
    struct EnemySpawn {
        int quantity;
        EEnemyType type;
    };

    struct SpawnPoint {
        int time;
        std::vector<EnemySpawn> enemies;
        bool spawned = false;
    };
}