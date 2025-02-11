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

#include "LevelManager.hpp"
#include "ECS/Scene/SceneManager.hpp"
#include "Network/Packets/Descriptors/PacketBonus/PacketBonus.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/Components/Shared/EventId.hpp"
#include "RType/Components/Shared/ProjectileInfo.hpp"
#include "RType/Services/BonusService/BonusService.hpp"
#include "RType/Services/EnemyService/EnemyService.hpp"
#include "RType/Systems/Network/Network.hpp"

//TODO: this code should be only in the server
namespace rtype::levels {

    void Level::endGame(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, bool isLose) {
        for (const auto &entity : entityManager.getEntities()) {
            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
            auto gameState = componentManager.getComponent<components::GameState>(entity);
            auto menuState = componentManager.getComponent<components::MenuState>(entity);
            auto ai = componentManager.getComponent<components::IA>(entity);
            auto projectileInfo = componentManager.getComponent<components::ProjectileInfo>(entity);

            if (projectileInfo) {
                entityManager.destroyEntity(entity);
                componentManager.removeAllComponent(entity);
                continue;
            }

            if (ai && isLose) {
                entityManager.destroyEntity(entity);
                componentManager.removeAllComponent(entity);
                continue;
            }

            if (menuState) {
                menuState->playerCount = 0;
                componentManager.addComponent<components::MenuState>(entity, *menuState, entityManager);
            }

            if (gameState) {
                componentManager.addComponent(entity, components::GameState{0, 0}, entityManager);
            }
            if (netCo) {
                network::TCPNetwork::getInstance().sendPacket(network::PacketEndGame(isLose), netCo->socket);
                entityManager.destroyEntity(entity);
                componentManager.removeAllComponent(entity);
            }
        }
        for (auto &spawn : this->_spawns) {
            spawn.spawned = false;
        }
        this->_started = false;
        this->_ended = false;
        systems::Network::globalNetId = 0;
        ecs::SceneManager::getInstance().changeScene(0);
    }

    void Level::process(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution dis(10, 500);

        if (!this->_started) {
            this->_start =  std::chrono::steady_clock::now();
            this->_started = true;
        }

        if (this->_ended) {
            return;
        }

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

                for (models::EBonusType bonus : spawn.bonuses) {
                    const auto y = static_cast<float>(dis(gen));
                    const auto x = static_cast<float>(dis(gen));


                    components::globalEventId.store(components::globalEventId.load() + 1);
                    services::BonusService::createBonus(entityManager, componentManager, bonus, {x, y, 0}, components::globalEventId.load());
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
            this->endGame(entityManager, componentManager, true);
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
                this->endGame(entityManager, componentManager, false);
                spdlog::info("Game win, all enemies are dead");
                return true;
            }
        }
        return false;
    }
}
