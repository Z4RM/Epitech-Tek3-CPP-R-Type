/*
** EPITECH PROJECT, 2025
** RType
** File description:
** monster spawner
*/

#include "MonsterSpawner.hpp"
#include <random>
#include "RType/Entities/Enemy.hpp"
#include "../Components/Shared/Position.hpp"
#include "../Components/Shared/Velocity.hpp"
#include "../Components/Shared/Size.hpp"
#include "../Components/Client/Sprite.hpp"

namespace rtype::systems {
    std::chrono::steady_clock::time_point MonsterSpawner::_lastSpawnTime = std::chrono::steady_clock::now();
    std::unordered_map<size_t, std::chrono::steady_clock::time_point> _lastShootTime; // Ajout du timer pour les tirs ennemis

    void MonsterSpawner::spawnMonster(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
#ifndef RTYPE_IS_CLIENT
        static int monsterId = 10;
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - _lastSpawnTime;

        for (auto &entity : entityManager.getEntities()) {
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

                _lastShootTime[monsterId] = now; // Initialisation du timer de tir
            }

            _lastSpawnTime = now;
        }

        for (auto &entity : entityManager.getEntities()) {
            auto enemyPos = componentManager.getComponent<components::Position>(entity);

            double shootCooldown = 2.0;
            if (_lastShootTime.find(entity) != _lastShootTime.end()) {
                std::chrono::duration<double> shootElapsed = now - _lastShootTime[entity];
                if (shootElapsed.count() < shootCooldown)
                    continue;
            }
            _lastShootTime[entity] = now;

            size_t projectileId = entityManager.createEntity();
            componentManager.addComponent<components::Position>(projectileId, {enemyPos->x - 10.0f, enemyPos->y, enemyPos->z});
            componentManager.addComponent<components::Velocity>(projectileId, {-3.0f, 0.0f, 0.0f});
            componentManager.addComponent<components::Size>(projectileId, {82.0f, 18.0f});

            components::Sprite projectileSprite = {
                {enemyPos->x - 10.0f, enemyPos->y, enemyPos->z},
                {82.0f, 18.0f},
                "assets/sprites/projectile/enemy-shots.png",
                {1},
                {1.0, 1.0},
                std::make_shared<sf::Texture>(),
                std::make_shared<sf::Sprite>(),
                165,
                82
            };
            projectileSprite.texture->loadFromFile(projectileSprite.path);
            projectileSprite.sprite->setTexture(*projectileSprite.texture);
            sf::IntRect textureRect(82, 165, 82, 18);
            projectileSprite.sprite->setTextureRect(textureRect);
            componentManager.addComponent<components::Sprite>(projectileId, projectileSprite);

        }
#endif
    }
}
