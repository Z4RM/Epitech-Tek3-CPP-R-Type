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

    class LevelManager {
    public:
        inline void registerLevel(std::shared_ptr<Level> level) { this->_levels.emplace_back(std::move(level)); }

        void processCurrentLevel(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) const;
        static LevelManager &getInstance();
        void changeLevel(int number);

    private:
        LevelManager() = default;
        std::shared_ptr<Level> _currentLevel = nullptr;
        std::vector<std::shared_ptr<Level>> _levels {};
    };
}
