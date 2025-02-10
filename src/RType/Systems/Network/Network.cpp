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
#include "handlers/ConnectHandler/ConnectHandler.hpp"
#include "handlers/EnnemiesDataHandler/EnnemiesDataHandler.hpp"
#include "handlers/LevelsRegisteredHandler/LevelsRegisteredHandler.hpp"
#include "handlers/PlayerCountHandler/PlayerCountHandler.hpp"
#include "handlers/PlayerDataHandler/PlayerDataHandler.hpp"
#include "handlers/PlayerShootHandler/PlayerShootHandler.hpp"
#include "handlers/StartGameHandler/StartGameHandler.hpp"
#include "handlers/WelcomeHandler/WelcomeHandler.hpp"
#include "handlers/EndGameHandler/EndGameHandler.hpp"
#include "Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp"
#include "Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.hpp"
#include "Network/Packets/Descriptors/PacketPlayerCounter/PacketPlayerCounter.hpp"
#include "RType/Entities/Player.hpp"

namespace rtype::systems {
    std::atomic<int> Network::globalNetId = 0;

    //TODO: check why this udp running entity is needed for closing correctly the client when the window close
    void Network::udpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        network::UDPNetwork &network = network::UDPNetwork::getInstance(Config::getInstance().getNetwork().server.port);

        if (!network.getStarted()) {
            try {
                if (!IS_SERVER) {
                    network.registerNetHandler(network::ENEMIES_DATA, std::make_unique<EnnemiesDataHandler>(componentManager, entityManager));
                }
                network.registerNetHandler(network::PLAYERS_DATA, std::make_unique<PlayerDataHandler>(componentManager, entityManager));
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
            bool inGame = false;

            //TODO: Maybe add a service or handle state differently to avoid loop like that in different part of the code
            for (const auto &entity: entityManager.getEntities()) {
                const auto gameState = componentManager.getComponent<components::GameState>(entity);

                if (gameState) {
                    if (gameState->isStarted)
                        inGame = true;
                    break;
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

                    if (vel && pos && size && netId && health) {
                        models::PlayerData pdata{*pos, *vel, *size, *netId, health->value};
                        models::EnemyData edata{*pos, *vel, *size, *netId, health->value};

                        if (actualPlayer && actualPlayer->value == true && !IS_SERVER) {
                            if (health->value <= 0)
                                continue;
                            playerDatas.emplace_back(pdata);
                        }

                        if (IS_SERVER && !ai) {
                            playerDatas.emplace_back(pdata);
                        } else if (IS_SERVER && ai) {
                            enemyDatas.emplace_back(edata);
                        }
                    }
                }
            }
            network::PacketPlayersData packetPlayersData(playerDatas);
            network::PacketEnemiesData packetEnemyDatas(enemyDatas);

            if (!IS_SERVER) {
                if (!playerDatas.empty())
                    network.sendPacket(packetPlayersData, network.getServerEndpoint());
            }
            else {
                for (const auto &entity : entityManager.getEntities()) {
                    const auto net = componentManager.getComponent<components::NetworkConnection>(entity);
                    if (net && net->endpoint.has_value()) {
                        if (!playerDatas.empty())
                            network.sendPacket(packetPlayersData, net->endpoint.value());
                        if (!enemyDatas.empty())
                            network.sendPacket(packetEnemyDatas, net->endpoint.value());
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
                                componentManager.addComponent<components::MenuState>(entity, *menuState);
                                stateUpdated = true;
                                if (playerDestroyed)
                                    break;
                            }

                            if (netCo && netCo->socket == socket) {
                                entityManager.destroyEntity(entity, componentManager);
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
                network.registerNetHandler(network::PLAYER_SHOOT, std::make_unique<PlayerShootHandler>(componentManager, entityManager));
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Unable to start TCP socket: {}", e.what());
            }
        }
    }
}
