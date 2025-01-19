/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Network.cpp
*/

#include "Network.hpp"

#include "Network/Packets/Descriptors/PacketNewPlayer/PacketNewPlayer.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "Network/UDPNetwork/UDPNetwork.hpp"

#ifndef RTYPE_IS_SERVER
#include "RType/Components/Client/Sprite.hpp"
#endif
#include "ECS/Scene/SceneManager.hpp"

#include "RType/Config/Config.hpp"
#include "Components.hpp"
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
                spdlog::error("Error while starting udp");
            }
        } else {
            for (auto &entity : entityManager.getEntities()) {
                auto stop = componentManager.getComponent<components::Running>(entity);

                if (stop && !network.getStop()) {
                    if (!stop->running) {
                        network.setStop(true);
                        componentManager.getComponent<components::Running>(udp)->running = false;
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

                    if (actualPlayer && actualPlayer->value == true && !IS_SERVER)
                        playerDatas.emplace_back(pdata);

                    if (IS_SERVER && !ai) {
                        playerDatas.emplace_back(pdata);
                    } else if (IS_SERVER && ai) {
                        enemyDatas.emplace_back(edata);
                    }
                }
            }

            if (!playerDatas.empty()) {
                network::PacketPlayersData packetPlayersData(playerDatas);

                if (!IS_SERVER) {
                    network.sendPacket(packetPlayersData, network.getServerEndpoint());
                }
                else {
                    for (const auto &entity : entityManager.getEntities()) {
                        const auto net = componentManager.getComponent<components::NetworkConnection>(entity);
                        if (net) {
                            if (net->endpoint.has_value()) {
                                network.sendPacket(packetPlayersData, net->endpoint.value());
                            }
                        }
                    }
                }
            }

            if (!enemyDatas.empty() && IS_SERVER) {
                network::PacketEnemiesData packetEnemyDatas(enemyDatas);
                for (const auto &entity : entityManager.getEntities()) {
                    const auto net = componentManager.getComponent<components::NetworkConnection>(entity);
                    if (net) {
                        if (net->endpoint.has_value()) {
                            network.sendPacket(packetEnemyDatas, net->endpoint.value());
                        }
                    }
                }
            }

            timer->expires_after(std::chrono::milliseconds(1));
            timer->async_wait([&entityManager, &componentManager, &network, timer](const asio::error_code& ec) {
                if (!ec) {
                    schedulePacketSending(entityManager, componentManager, network, timer);
                }
            });
        } catch (const std::exception &e) {
            spdlog::error("Exception in sendPackets: {}", e.what());
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
                                    spdlog::info("New player in the udp game with net id: {}", net->id);
                                    netCo->endpoint.emplace(endpoint);
                                }
                                if (netCo->endpoint.has_value() && netCo->endpoint == endpoint) {
                                    //*pos = data.pos;
                                    *vel = data.vel;
                                    *size = data.size;
                                }
                            }
                        }
                    }
                }
                else {
                    spdlog::error("Invalid packet players data received");
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
                                            }
                                        }

                                        if (!actualPlayer->value) {
                                            *vel = data.vel;
                                        }
                                        break;
                                    }
                                }
                            }

                            if (!created) {
                            spdlog::info("Creating new player in game");
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
                                    if (data.netId.id == net->id)
                                        isDead = false;
                                }
                                if (isDead) {
                                    spdlog::info("Destroying disconnected player");
                                    entityManager.destroyEntity(entity, componentManager);
                                }
                            }
                        }
                    } else {
                        spdlog::error("Invalid packet players data received");
                    }
            });


            network.addHandler(network::ENEMIES_DATA, [&network, &entityManager, &componentManager](std::unique_ptr<network::IPacket>
                packet, asio::ip::udp::endpoint endpoint) {
                    auto* enemiesData = dynamic_cast<network::PacketEnemiesData*>(packet.get());

                    if (enemiesData) {
                        for (const models::EnemyData &data: enemiesData->datas) {
                            bool created = false;

                            for (const auto &entity : entityManager.getEntities()) {
                                auto net = componentManager.getComponent<components::NetId>(entity);
                                auto ai = componentManager.getComponent<components::IA>(entity);

                                if (net && ai) {
                                    int netId = net->id;

                                    if (data.netId.id == netId) {
                                        created = true;
                                        const auto localPos = componentManager.getComponent<components::Position>(entity);
                                        const auto vel = componentManager.getComponent<components::Velocity>(entity);
                                        const auto health = componentManager.getComponent<components::Health>(entity);

                                        if (health) {
                                            if (data.health != health->value) {
                                                health->value = data.health;
                                                health->takeDamage(0);
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
                                            }
                                        }
                                        *vel = data.vel;
                                        break;
                                    }
                                }
                            }

                            if (!created) {
                            spdlog::info("Creating new enemy in game");
                            #ifndef RTYPE_IS_SERVER
                                components::Sprite sprite3 = {{600, 100, 0}, {33, 36}, "assets/sprites/enemy.gif", {1}};
                                rtype::entities::Enemy enemy(
                                    entityManager,
                                    componentManager,
                                    {600, 100, 0},
                                    {0, 0, 0},
                                    {64, 64},
                                    sprite3,
                                    {"", 0, 0},
                                    data.netId
                                );
                            #endif
                                continue;
                            }
                        }
                        for (const auto entity : entityManager.getEntities()) {
                            auto net = componentManager.getComponent<components::NetId>(entity);
                            auto ai = componentManager.getComponent<components::IA>(entity);

                            bool isDead = true;

                            if (net && net->id != 0 && ai) {
                                for (const models::EnemyData &data : enemiesData->datas) {
                                    if (data.netId.id == net->id)
                                        isDead = false;
                                }
                                if (isDead) {
                                    spdlog::info("Destroying disconnected enemy");
                                    entityManager.destroyEntity(entity, componentManager);
                                }
                            }
                        }
                    } else {
                        spdlog::error("Invalid packet players data received");
                    }
            });
        }
    }


    void Network::tcpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        static network::TCPNetwork network(Config::getInstance().getNetwork().server.port);
        static bool networkStartingSended = false;
        static std::map<int, std::shared_ptr<asio::ip::tcp::socket>> playersToSayWelcome = {};
        static auto tcp = entityManager.createEntity();
        static std::atomic<int> player_count = 0;

        if (!IS_SERVER && network.getStarted() && !networkStartingSended) {
            for (auto &entity : entityManager.getEntities()) {
                auto gameSate = componentManager.getComponent<components::GameState>(entity);
                if (gameSate && gameSate->isStarted) {
                    networkStartingSended = true;
                    network::PacketStartGame packet;
                    network.sendPacket(packet);
                }
            }
        }

        if (!network.getStarted()) {

            components::Running running = { true };
            componentManager.addComponent(tcp, running);
            try {
                network.registerOnPlayerDisconnect([&entityManager, &componentManager](std::shared_ptr<asio::ip::tcp::socket> socket) {
                    if (IS_SERVER) {
                        for (auto &entity: entityManager.getEntities()) {
                            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
                            if (netCo && netCo->socket == socket) {
                                entityManager.destroyEntity(entity, componentManager);
                            }
                        }
                        for (auto it = playersToSayWelcome.begin(); it != playersToSayWelcome.end(); ) {
                            if (it->second == socket) {
                                player_count.store(player_count.load() - 1);
                                it = playersToSayWelcome.erase(it);
                            } else {
                                ++it;
                            }
                        }
                        network::PacketPlayerCounter packetPCount(player_count.load());
                        for (auto &p : playersToSayWelcome) {
                            network.sendPacket(packetPCount, p.second);
                        }
                        spdlog::info("Player destroyed");
                    } else {
                        spdlog::info("Server have closed the connection");
                    }
                });

                if (IS_SERVER) {
                    network.addHandler(network::CONNECT, [&entityManager, &componentManager](std::unique_ptr<network::IPacket> packet,
                        std::shared_ptr<asio::ip::tcp::socket> socket) {
                            for (auto &entity: entityManager.getEntities()) {
                                auto gameState = componentManager.getComponent<components::GameState>(entity);
                                if (gameState && gameState->isStarted) {
                                    spdlog::info("New player joined the game but the game is already started");
                                    return;
                                }
                            }
                            if (player_count.load() < 4) {
                                player_count.store(player_count.load() + 1);
                                network::PacketPlayerCounter playerCount(player_count.load());
                                std::lock_guard guard(Network::playerIdMutex);
                                Network::playerId++;
                                playersToSayWelcome[playerId] = socket;
                                for (auto &p : playersToSayWelcome) {
                                    network.sendPacket(playerCount, p.second);
                                }
                                spdlog::info("New player created with net id: {}");
                            } else {
                                //todo: send packet game already started to the client
                            }
                    });

                    network.addHandler(network::START_GAME, [&entityManager, &componentManager]
                    (std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
                        for (auto &entity: entityManager.getEntities()) {
                            auto gameState = componentManager.getComponent<components::GameState>(entity);
                            if (gameState) {
                                if (gameState->isStarted)
                                    return;
                                gameState->isStarted = true;
                            }
                        }

                        playerId++;
                        #ifndef RTYPE_IS_CLIENT
                            rtype::entities::Enemy enemy(
                                entityManager,
                                componentManager,
                                {600, 100, 0},
                                {0, 0, 0},
                                {64, 64},
                                {playerId}
                            );
                        #endif
                        for (auto &player : playersToSayWelcome) {
                            #ifndef RTYPE_IS_CLIENT
                            rtype::entities::Player playerShip(
                            entityManager,
                            componentManager,
                            {0, 0, 0},
                            {0, 0, 0},
                            {64, 64},
                            {socket},
                            { player.first }, {PLAYER_SPEED});

                            network::PacketWelcome welcome(player.first);
                            network.sendPacket(welcome, player.second);
                            #endif
                        }
                    });
                } else {
                    network.addHandler(network::WELCOME, [&entityManager, &componentManager](std::unique_ptr<network::IPacket> packet,
                    std::shared_ptr<asio::ip::tcp::socket> socket) {
                        auto* packetWelcome = dynamic_cast<network::PacketWelcome*>(packet.get());

                        if (packetWelcome) {
                            spdlog::info("Server said welcome, net id is: {}", packetWelcome->netId);
                            ecs::SceneManager::getInstance().changeScene(1, true);
                            #ifdef RTYPE_IS_CLIENT
                            components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};
                            entities::Player player2(
                                    entityManager,
                                    componentManager,
                                    {0, 200, 0},
                                    {0, 0, 0},
                                    {64, 64},
                                    sprite2,
                                    {"", 0, 0},
                                    {packetWelcome->netId},
                                    {true}
                            );
                            #endif
                        }
                    });

                    network.addHandler(network::PLAYER_COUNT, [&entityManager, &componentManager]
                    (std::unique_ptr<network::IPacket> packet,
                    std::shared_ptr<asio::ip::tcp::socket> socket) {
                        auto* packetPlayerCounter = dynamic_cast<network::PacketPlayerCounter*>(packet.get());

                        if (packetPlayerCounter) {
                            //spdlog::info("New player joined the game");
                            int count = 0;

                            for (auto &entity: entityManager.getEntities()) {
                                auto gameState = componentManager.getComponent<components::GameState>(entity);
                                auto playerCount = componentManager.getComponent<components::Counter>(entity);
                                if (gameState && gameState->isStarted) {
                                    spdlog::info("New player joined the game but the game is already started");
                                    return;
                                }
                                if (playerCount && playerCount->name == "players") {
                                    spdlog::warn(packetPlayerCounter->_count);
                                    playerCount->update(packetPlayerCounter->_count);
                                }
                            }
                        }
                    });
                }

                network.start();
            } catch (std::exception &e) {
                spdlog::error("Error while starting tcp");
            }
        } else {
            for (auto &entity : entityManager.getEntities()) {
                auto stop = componentManager.getComponent<components::Running>(entity);

                if (stop && !network.getStop()) {
                    if (!stop->running) {
                        network.setStop(true);
                        componentManager.getComponent<components::Running>(tcp)->running = false;
                    }
                }
            }
        }
    }
}
