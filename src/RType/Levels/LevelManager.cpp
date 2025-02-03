/*
** EPITECH PROJECT, 2025
** RType
** File description:
** LevelManager
*/

#include "LevelManager.hpp"

namespace rtype::levels {

    void LevelManager::changeLevel(int number) {
        for (const auto &level : this->_levels) {
            if (level->getNumber() == number)
                this->_currentLevel = level;
        }
    }

    LevelManager &LevelManager::getInstance() {
        static LevelManager instance;

        return instance;
    }

    void LevelManager::processCurrentLevel(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) const {
        if (this->_currentLevel) {
            this->_currentLevel->process(entityManager, componentManager);
        }
    }
}