/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Network.cpp
*/

#include "Network.hpp"

#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "Network/UDPNetwork/UDPNetwork.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/Components/Client/Sprite.hpp"
#endif
#include "ECS/Scene/SceneManager.hpp"

#include "RType/Config/Config.hpp"
#include "Components.hpp"
#include "handlers/BonusHandler/BonusHandler.hpp"
#include "handlers/ConnectHandler/ConnectHandler.hpp"
#include "handlers/EnnemiesDataHandler/EnnemiesDataHandler.hpp"
#include "handlers/LevelsRegisteredHandler/LevelsRegisteredHandler.hpp"
#include "handlers/PlayerCountHandler/PlayerCountHandler.hpp"
#include "handlers/PlayerDataHandler/PlayerDataHandler.hpp"
#include "handlers/PlayerShootHandler/PlayerShootHandler.hpp"
#include "handlers/StartGameHandler/StartGameHandler.hpp"
#include "handlers/WelcomeHandler/WelcomeHandler.hpp"
#include "handlers/EndGameHandler/EndGameHandler.hpp"
#include "Network/Packets/Descriptors/PacketBonus/PacketBonus.hpp"
#include "Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp"
#include "Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.hpp"
#include "Network/Packets/Descriptors/PacketPlayerCounter/PacketPlayerCounter.hpp"
#include "RType/Components/Shared/Bonus.hpp"
#include "RType/Components/Shared/EventId.hpp"
#include "RType/Components/Shared/PlayerBonuses.hpp"
#include "RType/Components/Shared/ProjectileInfo.hpp"
#include "RType/Entities/Player.hpp"
#include "RType/Models/BonusData.hpp"
#include "RType/Models/ProjectileData.hpp"

namespace rtype::systems {
    std::atomic<int> Network::globalNetId = 0;

    void Network::udpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        network::UDPNetwork &network = network::UDPNetwork::getInstance(Config::getInstance().getNetwork().server.port);

        if (!network.getStarted()) {
            try {
                if (!IS_SERVER) {
                    network.registerNetHandler(network::ENEMIES_DATA, std::make_unique<EnnemiesDataHandler>(componentManager, entityManager));
                }
                network.registerNetHandler(network::PLAYERS_DATA, std::make_unique<PlayerDataHandler>(componentManager, entityManager));
                network.registerNetHandler(network::PLAYER_SHOOT, std::make_unique<PlayerShootHandler>(componentManager, entityManager));
            #ifdef RTYPE_IS_CLIENT
                network.registerNetHandler(network::BONUS_SPAWN, std::make_unique<BonusHandler>(componentManager, entityManager));
            #endif
                auto timer = std::make_shared<asio::steady_timer>(network.getIoContext());
                schedulePacketSending(entityManager, componentManager, network, timer);
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Unable to start UDP socket: {}", e.what());
            }
        }
    }

    void Network::schedulePacketSending(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
        network::UDPNetwork &network, std::shared_ptr<asio::steady_timer> timer) {
        try {
            std::vector<models::PlayerData> playerDatas;
            std::vector<models::EnemyData> enemyDatas;
            std::vector<models::BonusData> bonuses;
            std::vector<models::ProjectileData> projectiles;
            bool inGame = false;
            int actualPlayerNetId = -1;

            //TODO: Maybe add a service or handle state differently to avoid loop like that in different part of the code
            for (const auto &entity: entityManager.getEntities()) {
                const auto gameState = componentManager.getComponent<components::GameState>(entity);
                const auto actualPlayer = componentManager.getComponent<components::ActualPlayer>(entity);
                const auto netId = componentManager.getComponent<components::NetId>(entity);

                if (gameState) {
                    if (gameState->isStarted)
                        inGame = true;
                }
                if (actualPlayer && netId && actualPlayer->value == true) {
                    actualPlayerNetId = netId->id;
                }
            }

            if (inGame) {
                for (const auto& entity : entityManager.getEntities()) {
                    const auto netId = componentManager.getComponent<components::NetId>(entity);
                    const auto vel = componentManager.getComponent<components::Velocity>(entity);
                    const auto pos = componentManager.getComponent<components::Position>(entity);
                    const auto size = componentManager.getComponent<components::Size>(entity);
                    const auto actualPlayer = componentManager.getComponent<components::ActualPlayer>(entity);
                    const auto health = componentManager.getComponent<components::Health>(entity);
                    const auto ai = componentManager.getComponent<components::IA>(entity);
                    auto event = componentManager.getComponent<components::EventId>(entity);
                    auto bonus = componentManager.getComponent<components::Bonus>(entity);
                    auto projectileInfo = componentManager.getComponent<components::ProjectileInfo>(entity);
                    auto playerBonuses = componentManager.getComponent<components::PlayerBonuses>(entity);

                    if (projectileInfo && event) {

                        if (!IS_SERVER && event->netIdEmitter == actualPlayerNetId) {
                            models::ProjectileData projectileData = {*event, *projectileInfo, *pos};
                            projectiles.emplace_back(projectileData);
                        } else if (IS_SERVER) {
                            models::ProjectileData projectileData = {*event, *projectileInfo, *pos};
                            projectiles.emplace_back(projectileData);
                        }
                    }
                    if (IS_SERVER && pos && event && bonus) {
                        models::BonusData bonusData = {{pos->x, pos->y}, event->value, bonus->type};
                        bonuses.emplace_back(bonusData);
                    }
                    if (vel && pos && size && netId && health) {
                        std::vector<models::EBonusType> actualPlayerBonuses = {};
                        #ifdef RTYPE_IS_SERVER
                        if (playerBonuses) {
                            actualPlayerBonuses = playerBonuses->bonuses;
                        }
                        #endif
                        models::PlayerData pdata{*pos, *vel, *size, *netId, health->value, actualPlayerBonuses};
                        models::EnemyData edata{*pos, *vel, *size, *netId, health->value};

                        if (actualPlayer && actualPlayer->value == true && !IS_SERVER) {
                            if (health->value <= 0)
                                continue;
                            playerDatas.emplace_back(pdata);
                        }

                        if (IS_SERVER && !ai) {
                            playerDatas.emplace_back(pdata);
                        } else if (IS_SERVER && ai) {
                            edata.type = ai->type;
                            enemyDatas.emplace_back(edata);
                        }
                    }
                }
                network::PacketPlayersData packetPlayersData(playerDatas);
                network::PacketEnemiesData packetEnemyDatas(enemyDatas);

                if (!IS_SERVER) {
                    if (!playerDatas.empty())
                        network.sendPacket(packetPlayersData, network.getServerEndpoint());
                    for (auto &projectile: projectiles) {
                        network::PacketPlayerShoot packetPlayerShoot(projectile.event.netIdEmitter, projectile
                        .projectileInfo.isSuperProjectile, projectile.event.value, projectile.pos, projectile.projectileInfo.isPlayer);
                        network.sendPacket(packetPlayerShoot, network.getServerEndpoint());
                    }
                }
                else {
                    for (const auto &entity : entityManager.getEntities()) {
                        const auto net = componentManager.getComponent<components::NetworkConnection>(entity);
                        auto netId = componentManager.getComponent<components::NetId>(entity);
                        if (net && net->endpoint.has_value()) {
                            if (!playerDatas.empty())
                                network.sendPacket(packetPlayersData, net->endpoint.value());
                            if (!enemyDatas.empty())
                                network.sendPacket(packetEnemyDatas, net->endpoint.value());
                            for (auto &bonus: bonuses) {
                                network::PacketBonus packetBonus(bonus.bonusType, bonus.pos, bonus.eventId);
                                network.sendPacket(packetBonus, net->endpoint.value());
                            }
                            for (auto &projectile : projectiles) {
                                network::PacketPlayerShoot packetPlayerShoot(projectile.event.netIdEmitter, projectile
                                .projectileInfo.isSuperProjectile, projectile.event.value, projectile.pos);
                                network.sendPacket(packetPlayerShoot, net->endpoint.value());
                            }
                        }
                    }
                }
            }
            timer->expires_after(std::chrono::milliseconds(16));
            timer->async_wait([&entityManager, &componentManager, &network, timer](const asio::error_code& ec) {
                if (!ec) {
                    schedulePacketSending(entityManager, componentManager, network, timer);
                }
            });
        } catch (const std::exception &e) {
            spdlog::error("Unable to send packet: {}", e.what());
        }
    }

    void Network::tcpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        network::TCPNetwork &network = network::TCPNetwork::getInstance(Config::getInstance().getNetwork().server.port);

        if (!network.getStarted()) {

            try {
                network.registerOnPlayerDisconnect([&entityManager, &componentManager, &network](std::shared_ptr<asio::ip::tcp::socket> socket) {
                    if (IS_SERVER) {
                        int newCount = 0;
                        bool stateUpdated = false;
                        bool playerDestroyed = false;

                        for (auto &entity: entityManager.getEntities()) {
                            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
                            auto menuState = componentManager.getComponent<components::MenuState>(entity);

                            if (menuState) {
                                menuState->playerCount -= 1;
                                newCount = menuState->playerCount;
                                componentManager.addComponent<components::MenuState>(entity, *menuState, entityManager);
                                stateUpdated = true;
                                if (playerDestroyed)
                                    break;
                            }

                            if (netCo && netCo->socket == socket) {
                                entityManager.destroyEntity(entity);
                                componentManager.removeAllComponent(entity);
                                playerDestroyed = true;
                                spdlog::info("Player destroyed");
                                if (stateUpdated)
                                    break;
                            }
                        }

                        network::PacketPlayerCounter packetPCount(newCount);
                        for (auto &entity : entityManager.getEntities()) {
                            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);

                            if (netCo) {
                                network.sendPacket(packetPCount, netCo->socket);
                            }
                        }
                    } else {
                        spdlog::info("Server has closed the connection");
                    }
                });

#ifndef RTYPE_IS_CLIENT
                    network.registerNetHandler(network::CONNECT, std::make_unique<ConnectHandler>(componentManager, entityManager));
                    network.registerNetHandler(network::START_GAME, std::make_unique<StartGameHandler>(componentManager, entityManager));
 #else
                    network.registerNetHandler(network::WELCOME, std::make_unique<WelcomeHandler>(componentManager, entityManager));
                    network.registerNetHandler(network::PLAYER_COUNT, std::make_unique<PlayerCountHandler>(componentManager, entityManager));
                    network.registerNetHandler(network::LEVELS_REGISTERED, std::make_unique<LevelsRegisteredHandler>(componentManager, entityManager));
                    network.registerNetHandler(network::END_GAME, std::make_unique<EndGameHandler>(componentManager, entityManager));
#endif
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Unable to start TCP socket: {}", e.what());
            }
        }
    }
}
