/*
** EPITECH PROJECT, 2025
** RType
** File description:
** LevelManager
*/

#include "LevelManager.hpp"

#include <spdlog/spdlog.h>

namespace rtype::levels {

    void LevelManager::changeLevel(int number) {
        std::lock_guard lock(_mutex);

        for (const auto &level : this->_levels) {
            if (level->getNumber() == number) {
                this->_currentLevel = level;
                return;
            }
        }
        this->_currentLevel = nullptr;
        spdlog::error("Bad level selected");
    }

    LevelManager &LevelManager::getInstance() {
        static LevelManager instance;

        return instance;
    }

    void LevelManager::processCurrentLevel(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        std::lock_guard lock(_mutex);

        if (this->_currentLevel) {
            this->_currentLevel->process(entityManager, componentManager);
        }
    }
}
