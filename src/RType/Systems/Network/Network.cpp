/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Network.cpp
*/

#include "Network.hpp"

#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "Network/UDPNetwork/UDPNetwork.hpp"

#ifndef RTYPE_IS_SERVER
#include "RType/Components/Client/Sprite.hpp"
#endif
#include "ECS/Scene/SceneManager.hpp"

#include "RType/Config/Config.hpp"
#include "Components.hpp"
#include "handlers/ConnectHandler/ConnectHandler.hpp"
#include "handlers/EnnemiesDataHandler/EnnemiesDataHandler.hpp"
#include "handlers/PlayerCountHandler/PlayerCountHandler.hpp"
#include "handlers/PlayerShootHandler/PlayerShootHandler.hpp"
#include "handlers/StartGameHandler/StartGameHandler.hpp"
#include "handlers/WelcomeHandler/WelcomeHandler.hpp"
#include "Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp"
#include "Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.hpp"
#include "Network/Packets/Descriptors/PacketPlayerCounter/PacketPlayerCounter.hpp"
#include "Network/Packets/Descriptors/PacketStartGame/PacketStartGame.hpp"
#include "RType/Components/Shared/Counter.hpp"
#include "RType/Entities/Enemy.hpp"
#include "RType/Entities/Player.hpp"

namespace rtype::systems {
    int Network::playerId = 0;
    std::mutex Network::playerIdMutex;
    std::atomic<int> Network::globalNetId = 0;

    //TODO: check why this udp running entity is needed for closing correctly the client when the window close
    void Network::udpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        static network::UDPNetwork network(Config::getInstance().getNetwork().server.port);
        static auto udp = entityManager.createEntity();

        if (!network.getStarted()) {
            components::Running running = { true };
            componentManager.addComponent(udp, running);
            try {
                addUdpHandlers(network, entityManager, componentManager);
                auto timer = std::make_shared<asio::steady_timer>(network.getIoContext());
                schedulePacketSending(entityManager, componentManager, network, timer);
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Unable to start UDP socket: {}", e.what());
            }
        } else {
            for (auto &entity : entityManager.getEntities()) {
                auto stop = componentManager.getComponent<components::Running>(entity);

                if (stop && !network.getStop()) {
                    if (!stop->running) {
                        network.setStop(true);
                        auto r = componentManager.getComponent<components::Running>(udp);
                        r->running = false;
                        componentManager.addComponent<components::Running>(udp, *r);
                    }
                }
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
                    const auto dead = componentManager.getComponent<components::Dead>(entity);

                    if (vel && pos && size && netId && health) {
                        models::PlayerData pdata{*pos, *vel, *size, *netId, health->value};
                        models::EnemyData edata{*pos, *vel, *size, *netId, health->value};

                        if (actualPlayer && actualPlayer->value == true && !IS_SERVER) {
                            if (dead)
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

    void Network::addUdpHandlers(network::UDPNetwork &network, ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        if (IS_SERVER) {
            network.addHandler(network::PLAYERS_DATA, [&network, &entityManager, &componentManager](std::unique_ptr<network::IPacket>
            packet, asio::ip::udp::endpoint endpoint) {
                auto* playersData = dynamic_cast<network::PacketPlayersData*>(packet.get());

                /** UPDATING THE GAME **/
                if (playersData) {
                    for (models::PlayerData &data : playersData->datas) {
                        for (const auto &entity : entityManager.getEntities()) {
                            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
                            auto pos = componentManager.getComponent<components::Position>(entity);
                            auto vel = componentManager.getComponent<components::Velocity>(entity);
                            auto size = componentManager.getComponent<components::Size>(entity);
                            auto net = componentManager.getComponent<components::NetId>(entity);

                            if (pos && vel && size && net && netCo) {
                                if (!netCo->endpoint.has_value() && net->id == data.netId.id) {
                                    spdlog::info("New player in the game with network ID {}", net->id);
                                    componentManager.addComponent<components::NetworkConnection>(entity, {netCo->socket, endpoint});
                                }
                                if (netCo->endpoint.has_value() && netCo->endpoint == endpoint) {
                                    //*pos = data.pos;
                                    *vel = data.vel;
                                    *size = data.size;
                                    componentManager.addComponent<components::Velocity>(entity, *vel);
                                    componentManager.addComponent<components::Size>(entity, *size);
                                }
                            }
                        }
                    }
                }
                else {
                    spdlog::error("Invalid player data packet received");
                }
            });

        } else {
            network.addHandler(network::PLAYERS_DATA, [&network, &entityManager, &componentManager](std::unique_ptr<network::IPacket>
                packet, asio::ip::udp::endpoint endpoint) {
                    auto* playersData = dynamic_cast<network::PacketPlayersData*>(packet.get());

                    if (playersData) {
                        for (const models::PlayerData &data: playersData->datas) {
                            bool created = false;

                            for (const auto &entity : entityManager.getEntities()) {
                                auto net = componentManager.getComponent<components::NetId>(entity);
                                auto actualPlayer = componentManager.getComponent<components::ActualPlayer>(entity);

                                if (net && actualPlayer) {
                                    int netId = net->id;

                                    if (data.netId.id == netId) {
                                        created = true;
                                        const auto localPos = componentManager.getComponent<components::Position>(entity);
                                        const auto vel = componentManager.getComponent<components::Velocity>(entity);
                                        const auto health = componentManager.getComponent<components::Health>(entity);

                                        if (health) {
                                            if (data.health != health->value) {
                                                health->setHealth(data.health);
                                                health->_elapsedDamage = std::chrono::steady_clock::now();
                                                if (data.health <= 0)
                                                    componentManager.addComponent<components::Dead>(entity, { true });
                                                componentManager.addComponent<components::Health>(entity, *health);
                                            }
                                        }

                                        if (localPos) {
                                            float distance = std::sqrt(
                                            std::pow(data.pos.x - localPos->x, 2) +
                                            std::pow(data.pos.y - localPos->y, 2) +
                                            std::pow(data.pos.z - localPos->z, 2)
                                            );
                                            const float positionThreshold = 0.1f;
                                            if (distance > positionThreshold) {
                                                *localPos = data.pos;
                                                componentManager.addComponent<components::Position>(entity, *localPos);
                                            }
                                        }

                                        if (!actualPlayer->value) {
                                            *vel = data.vel;
                                            componentManager.addComponent<components::Velocity>(entity, *vel);
                                        }
                                        break;
                                    }
                                }
                            }

                            if (!created) {
                            spdlog::debug("Creating new player in the game");
                            #ifndef RTYPE_IS_SERVER
                                components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};
                                entities::Player player2(
                                        entityManager,
                                        componentManager,
                                        {0, 200, 0},
                                        {0, 0, 0},
                                        {64, 64},
                                        sprite2,
                                        {"", 0, 0},
                                        data.netId
                                );
                            #endif
                            }
                        }
                        for (const auto entity : entityManager.getEntities()) {
                            auto net = componentManager.getComponent<components::NetId>(entity);
                            auto actualPlayer = componentManager.getComponent<components::ActualPlayer>(entity);
                            bool isDead = true;

                            if (net && actualPlayer) {
                                for (const models::PlayerData &data : playersData->datas) {
                                    if (data.netId.id == net->id) {
                                        isDead = false;
                                    }
                                }
                                if (isDead) {
                                    spdlog::debug("Destroying disconnected player");
                                    entityManager.destroyEntity(entity, componentManager);
                                }
                            }
                        }
                    } else {
                        spdlog::error("Invalid player data packet received");
                    }
            });

            network.registerNetHandler(network::ENEMIES_DATA, std::make_unique<EnnemiesDataHandler>(componentManager, entityManager));

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

                if (IS_SERVER) {
                    network.registerNetHandler(network::CONNECT, std::make_unique<ConnectHandler>(componentManager, entityManager));
                    network.registerNetHandler(network::START_GAME, std::make_unique<StartGameHandler>(componentManager, entityManager));
                } else {
                    network.registerNetHandler(network::WELCOME, std::make_unique<WelcomeHandler>(componentManager, entityManager));
                    network.registerNetHandler(network::PLAYER_COUNT, std::make_unique<PlayerCountHandler>(componentManager, entityManager));
                }
                network.registerNetHandler(network::PLAYER_SHOOT, std::make_unique<PlayerShootHandler>(componentManager, entityManager));
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Unable to start TCP socket: {}", e.what());
            }

        } else {
            for (auto &entity : entityManager.getEntities()) {
                auto stop = componentManager.getComponent<components::Running>(entity);

                if (stop && !network.getStop()) {
                    if (!stop->running) {
                        network.setStop(true);
                    }
                }
            }
        }
    }
}
