/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ILevel
*/

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "RType/Models/EEnemyType.hpp"
#include "RType/Models/Spawn.hpp"
#include <chrono>

namespace rtype::levels {

    class Level {
    public:
        Level(int duration, std::vector<models::SpawnPoint> &spawns, int number) : _duration(duration), _spawns(std::move
        (spawns)), _number(number) {};

        [[nodiscard]] int getNumber() const { return this->_number; }

        void process();

    private:
        bool _started = false;
        bool _ended = false;
        int _number;
        int _duration = 0;

        long _prevElapsed = -1;

        std::vector<models::SpawnPoint> _spawns {};
        std::chrono::time_point<std::chrono::steady_clock> _start;
    };
}
