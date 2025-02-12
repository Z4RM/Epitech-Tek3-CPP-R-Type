/*
** EPITECH PROJECT, 2025
** RType
** File description:
** LevelManager
*/

#pragma once
#include <vector>
#include <memory>

#include "Level.hpp"

namespace rtype::levels {

    /** @brief Singleton used for managing levels **/
    class LevelManager {
    public:
        inline void registerLevel(std::shared_ptr<Level> level) {
            std::lock_guard lock(_mutex);
            this->_levels.emplace_back(std::move(level));
        }
        inline std::vector<std::shared_ptr<Level>> &getLevels() {
            std::lock_guard lock(_mutex);
            return this->_levels;
        }
        inline void setLevels(std::vector<std::shared_ptr<Level>> &levels) {
            std::lock_guard lock(_mutex);
            this->_levels = std::move(levels);
        }

        void processCurrentLevel(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);
        static LevelManager &getInstance();
        void changeLevel(int number);

        void reset() {
            std::lock_guard lock(_mutex);

            this->_currentLevel = nullptr;
            this->_levels = {};
        }

    private:
        LevelManager() = default;
        std::shared_ptr<Level> _currentLevel = nullptr;
        std::vector<std::shared_ptr<Level>> _levels {};
        std::mutex _mutex;
    };
}
