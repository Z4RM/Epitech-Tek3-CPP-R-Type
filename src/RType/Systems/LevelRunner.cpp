/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Level runner
*/

#include "LevelRunner.hpp"

#include <spdlog/spdlog.h>

#include "RType/Components/Server/AiShoot.hpp"
#include "RType/Components/Shared/GameState.hpp"
#include "RType/Components/Shared/NetId.hpp"
#include "RType/Levels/LevelManager.hpp"
#include "RType/Services/ProjectileService/ProjectileService.hpp"

namespace rtype::systems {
    void LevelRunner::process(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        bool isStarted = false;

        for (const auto &entity: entityManager.getEntities()) {
            auto gameState = componentManager.getComponent<components::GameState>(entity);

            if (gameState && gameState->isStarted) {
                isStarted = true;
                levels::LevelManager::getInstance().processCurrentLevel(entityManager, componentManager);

                break;
            }
        }
        for (auto &entity: entityManager.getEntities()) {
            auto aiShoot = componentManager.getComponent<components::AiShoot>(entity);
            auto enemyPos = componentManager.getComponent<components::Position>(entity);
            auto enemyNetId = componentManager.getComponent<components::NetId>(entity);

            if (isStarted) {
                if (aiShoot && enemyPos && enemyNetId) {
                    auto now = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsed = now - aiShoot->clock;

                    if (elapsed.count() >= aiShoot->cooldown) {
                        components::globalEventId.store(components::globalEventId.load() + 1);
                        components::EventId event;
                        event.value = components::globalEventId.load();
                        event.netIdEmitter = enemyNetId->id;
                        services::ProjectileService::createEnemyProjectile(entityManager, componentManager, enemyPos, event);
                        aiShoot->clock = std::chrono::steady_clock::now();
                        componentManager.addComponent<components::AiShoot>(entity, *aiShoot, entityManager);
                    }
                }
            }
        }
    }
}
