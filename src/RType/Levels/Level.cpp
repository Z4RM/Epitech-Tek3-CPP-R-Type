/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Level
*/

#include "Level.hpp"
#include <chrono>
#include <spdlog/spdlog.h>

namespace rtype::levels {
    void Level::process() {

        if (!this->_started) {
            this->_start =  std::chrono::steady_clock::now();
            this->_started = true;
        }

        if (this->_ended)
            return;

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - this->_start).count();

        if (this->_prevElapsed < elapsed) {
            this->_prevElapsed = elapsed;
            spdlog::info("{} seconds elapsed", elapsed);
        }

        if (elapsed >= this->_duration) {
            this->_ended = true;
            spdlog::info("Level ended");
            return;
        }

        for (auto &spawn : this->_spawns) {
            if (!spawn.spawned && elapsed >= spawn.time) {
                spdlog::info("Spawning spawn point");
                for (auto &enemySpawn : spawn.enemies) {
                    spdlog::info("Spawning {} ennemies of type {}", enemySpawn.quantity, std::to_string(enemySpawn.type));
                }
                spawn.spawned = true;
            }
        }
    }

}
