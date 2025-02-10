/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Level
*/

#include "Level.hpp"
#include <chrono>
#include <spdlog/spdlog.h>
#include <random>
#include <Network/Packets/Descriptors/PacketEndGame/PacketEndGame.hpp>

#include "ECS/Scene/SceneManager.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/Services/EnemyService/EnemyService.hpp"

//TODO: this code should be only in the server
namespace rtype::levels {
    void Level::process(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution dis(10, 500);

        if (!this->_started) {
            this->_start =  std::chrono::steady_clock::now();
            this->_started = true;
        }

        if (this->_ended)
            return;

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - this->_start).count();

        if (this->_prevElapsed < elapsed) {
            this->_prevElapsed = elapsed;
            spdlog::debug("{} seconds elapsed", elapsed);
        }

        //IF WE WANT TO ADD A TIME LIMIT TO THE PLAYER
        /*if (elapsed >= this->_duration) {
            this->_ended = true;
            spdlog::debug("Level ended");
            return;
        }*/

        if (this->isGameEnded(entityManager, componentManager)) {
            this->_ended = true;
            return;
        }

        for (auto &spawn : this->_spawns) {
            if (!spawn.spawned && elapsed >= spawn.time) {
                spdlog::debug("Spawning spawn point");

                for (auto &enemySpawn : spawn.enemies) {
                    spdlog::debug("Spawning {} ennemies of type {}", enemySpawn.quantity, std::to_string(enemySpawn.type));
                    for (int i = 0; i < enemySpawn.quantity; i++) {
                        const auto y = static_cast<float>(dis(gen));
                        services::EnemyService::createEnemy(entityManager, componentManager, {800, y, 0});
                    }
                }
                spawn.spawned = true;
            }
        }
    }

    bool Level::isGameEnded(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        int playerAliveCount = 0;
        int spawnedCount = 0;
        int enemyAliveCount = 0;

        for (const auto &entity : entityManager.getEntities()) {
            auto health = componentManager.getComponent<components::Health>(entity);
            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);

            if (netCo && health) {
                if (health->value > 0) {
                    playerAliveCount += 1;
                }
            }
        }

        if (playerAliveCount <= 0) {
            for (const auto &entity : entityManager.getEntities()) {
                auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
                auto gameState = componentManager.getComponent<components::GameState>(entity);
                auto ai = componentManager.getComponent<components::IA>(entity);

                if (ai) {
                    entityManager.destroyEntity(entity, componentManager);
                }

                if (gameState) {
                    componentManager.addComponent(entity, components::GameState{0, gameState->playerCount});
                }

                if (netCo) {
                    network::TCPNetwork::getInstance().sendPacket(network::PacketEndGame(true), netCo->socket);
                }
            }
            spdlog::info("Game lose, all players are dead");
            return true;
        }

        for (const auto &spawn: this->_spawns) {
            if (spawn.spawned)
                spawnedCount += 1;
        }

        if (spawnedCount >= this->_spawns.size()) {
            for (const auto &entity : entityManager.getEntities()) {
                auto ai = componentManager.getComponent<components::IA>(entity);

                if (ai) {
                    enemyAliveCount += 1;
                }
            }
            if (enemyAliveCount <= 0) {
                for (const auto &entity : entityManager.getEntities()) {
                    auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
                    auto gameState = componentManager.getComponent<components::GameState>(entity);

                    if (gameState) {
                        componentManager.addComponent(entity, components::GameState{0, gameState->playerCount});
                    }
                    if (netCo) {
                        network::TCPNetwork::getInstance().sendPacket(network::PacketEndGame(false), netCo->socket);
                    }
                }
                spdlog::info("Game win, all enemies are dead");
                return true;
            }
        }
        return false;
    }
}
