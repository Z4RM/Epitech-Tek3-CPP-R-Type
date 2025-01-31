/*
** EPITECH PROJECT, 2025
** RType
** File description:
** monster spawner
*/

#include <random>
#include "RType/ModeManager/ModeManager.hpp"
#include "RType/Entities/Enemy.hpp"
#include "MonsterSpawner.hpp"

namespace rtype::systems {
    std::chrono::steady_clock::time_point MonsterSpawner::_lastSpawnTime = std::chrono::steady_clock::now();

    void MonsterSpawner::spawnMonster(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        if (IS_SERVER) {
            static int monsterId = 10;
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = now - _lastSpawnTime;

            for (auto &entity: entityManager.getEntities()) {
                auto state = componentManager.getComponent<components::GameState>(entity);

                if (state) {
                    if (!state->isStarted) {
                        return;
                    }
                }
            }

            if (elapsed.count() >= 4.0 || monsterId == 0) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(1, 4);

                int numberOfMonsters = dis(gen);

                for (int i = 0; i < numberOfMonsters; ++i) {
                    monsterId++;
                    std::uniform_int_distribution<> pos(5, 600);
                    float y = pos(gen);

                    rtype::entities::Enemy enemy(
                            entityManager,
                            componentManager,
                            {700, y, 0},
                            {0, 0, 0},
                            {64, 64},
                            {monsterId}
                    );
                }

                _lastSpawnTime = now;
            }
        }
    }
}