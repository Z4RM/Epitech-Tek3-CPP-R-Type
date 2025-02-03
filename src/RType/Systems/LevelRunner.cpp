/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Level runner
*/

#include "LevelRunner.hpp"

#include "RType/Components/Shared/GameState.hpp"
#include "RType/Levels/LevelManager.hpp"

namespace rtype::systems {
    void LevelRunner::process(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        for (const auto &entity: entityManager.getEntities()) {
            auto gameState = componentManager.getComponent<components::GameState>(entity);

            if (gameState && gameState->isStarted) {
                levels::LevelManager::getInstance().processCurrentLevel(entityManager, componentManager);
            }
        }
    }

}