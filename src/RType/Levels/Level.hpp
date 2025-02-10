/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ILevel
*/

#pragma once

#include <utility>
#include <vector>
#include "RType/Models/Spawn.hpp"
#include <chrono>
#include "ECS.hpp"

namespace rtype::levels {

    /** @brief Represent in game Level **/
    class Level {
    public:
        /** @brief Level constructor
         * @param duration duration of the level
         * @param spawns spawns of the level
         * @param number number of the level (id)
         * **/
        Level(int duration, std::vector<models::SpawnPoint> &spawns, int number) : _duration(duration), _spawns(std::move
        (spawns)), _number(number) {};

        [[nodiscard]] int getNumber() const { return this->_number; }

        /** @brief process the level **/
        void process(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);

    private:
        bool _started = false; ///< used to know if the level have started
        bool _ended = false; ///< used to know if the level have ended
        int _number; ///< number of the level
        int _duration = 0; ///< duration of the level

        long _prevElapsed = -1; ///< second count

        std::vector<models::SpawnPoint> _spawns {}; ///< spawns of the level stored
        std::chrono::time_point<std::chrono::steady_clock> _start; ///< start clock of the level

        /** @brief Used to know of a current level have ended
         * @param entityManager entityManager of the game
         * @param componentManager componentManager of the game
         * @return true if ended, false otherwise
         * **/
        bool isGameEnded(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);

        void restoreServerState();
    };
}
