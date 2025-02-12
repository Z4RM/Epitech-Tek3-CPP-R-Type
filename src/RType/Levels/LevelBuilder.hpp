/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Level builder
*/

#pragma once
#include "Level.hpp"
#include "RType/Models/Spawn.hpp"

namespace rtype::levels {

    /** @brief Builder used to build a level
     * configurable params are number, duration and spawns
     * **/
    class LevelBuilder {
    public:
        LevelBuilder() = default;

        LevelBuilder &setDuration(int duration) {
            this->_duration = duration;
            return *this;
        }
        LevelBuilder &addSpawnPoint(models::SpawnPoint &point) {
            this->_spawns.emplace_back(point);
            return *this;
        }
        LevelBuilder &setNumber(int number) {
            this->_number = number;
            return *this;
        }
        Level build() {
            return Level(_duration, _spawns, _number);
        };
    private:
        int _number = 0;
        int _duration = 0;
        std::vector<models::SpawnPoint> _spawns {};
    };
}