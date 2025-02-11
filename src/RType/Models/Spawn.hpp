/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Spawn
*/

#pragma once

#include <vector>
#include "./EEnemyType.hpp"
#include <nlohmann/json.hpp>

#include "EBonusType.hpp"

namespace rtype::models {
    struct EnemySpawn {
        int quantity;
        EEnemyType type;
    };

    struct SpawnPoint {
        int time;
        std::vector<EnemySpawn> enemies;
        std::vector<EBonusType> bonuses = {};
        bool spawned = false;
    };
}
