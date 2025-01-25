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
                                        [](size_t id) {},
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
                                        *vel = data.vel;
                                        componentManager.addComponent<components::Velocity>(entity, *vel);
                                        break;
                                    }
                                }
                            }

                            if (!created) {
                            spdlog::debug("Creating new enemy in the game");
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
                                    spdlog::debug("Destroying disconnected enemy");
                                    entityManager.destroyEntity(entity, componentManager);
                                }
                            }
                        }
                    } else {
                        spdlog::error("Invalid player data packet received");
                    }
            });
        }
    }


    void Network::tcpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        network::TCPNetwork &network = network::TCPNetwork::getInstance(Config::getInstance().getNetwork().server.port);
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
                network.registerOnPlayerDisconnect([&entityManager, &componentManager, &network](std::shared_ptr<asio::ip::tcp::socket> socket) {
                    if (IS_SERVER) {
                        for (auto &entity: entityManager.getEntities()) {
                            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
                            if (netCo && netCo->socket == socket) {
                                entityManager.destroyEntity(entity, componentManager);
                                spdlog::info("Player destroyed");
                                break;
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
                    } else {
                        spdlog::info("Server has closed the connection");
                    }
                });

                if (IS_SERVER) {
                    network.addHandler(network::PLAYER_SHOOT, [&entityManager, &componentManager, &network]
                    (std::unique_ptr<network::IPacket> packet,
                    std::shared_ptr<asio::ip::tcp::socket> socket) {
                        auto* packetPlayerShoot = dynamic_cast<network::PacketPlayerShoot*>(packet.get());

                        if (packetPlayerShoot) {
                            for (auto &entity: entityManager.getEntities()) {
                                const auto dead = componentManager.getComponent<components::Dead>(entity);

                                if (dead)
                                    continue;

                                auto netco = componentManager.getComponent<components::NetworkConnection>(entity);
                                auto netId = componentManager.getComponent<components::NetId>(entity);
                                auto playerPos = componentManager.getComponent<components::Position>(entity);

                                if (netco && netId) {
                                    if (netId->id == packetPlayerShoot->netId) {
                                        size_t projectileId = entityManager.createEntity();

                                        components::Velocity vel = {2.0, 0.0, 0.0};
                                        components::Position pos = {playerPos->x + 10.0f, playerPos->y, playerPos->z};
                                        componentManager.addComponent<components::Velocity>(projectileId, vel);
                                        componentManager.addComponent<components::Position>(projectileId, pos);
                                        componentManager.addComponent<components::Size>(projectileId, {10.0f, 10.0f});
                                        componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}});
                                        componentManager.addComponent<components::Speed>(projectileId, {250});
                                        componentManager.addComponent<components::Damage>(projectileId, {20});
                                        componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true});
                                        network::PacketPlayerShoot newPacket(packetPlayerShoot->netId);
                                        for (auto &p : playersToSayWelcome) {
                                            if (socket != p.second)
                                                network.sendPacket(newPacket, p.second);
                                        }
                                    }
                                }
                            }
                        }
                    });

                    network.addHandler(network::CONNECT, [&entityManager, &componentManager, &network](std::unique_ptr<network::IPacket> packet,
                                                                                                       std::shared_ptr<asio::ip::tcp::socket> socket) {
                            for (auto &entity: entityManager.getEntities()) {
                                auto gameState = componentManager.getComponent<components::GameState>(entity);
                                if (gameState && gameState->isStarted) {
                                    spdlog::warn("New player joined the game, but it's already started");
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
                                spdlog::info("New player created with network ID {}", playerId);
                            } else {
                                //todo: send packet game already started to the client
                            }
                    });

                    network.addHandler(network::START_GAME, [&entityManager, &componentManager, &network]
                    (std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
                        for (auto &entity: entityManager.getEntities()) {
                            auto gameState = componentManager.getComponent<components::GameState>(entity);
                            if (gameState) {
                                if (gameState->isStarted)
                                    return;
                                gameState->isStarted = true;
                                componentManager.addComponent<components::GameState>(entity, *gameState);
                            }
                        }

                        for (auto &player : playersToSayWelcome) {
                            #ifndef RTYPE_IS_CLIENT
                            rtype::entities::Player playerShip(
                            entityManager,
                            componentManager,
                            {0, 0, 0},
                            {0, 0, 0},
                            {64, 64},
                            {player.second},
                            { player.first }, {PLAYER_SPEED});

                            network::PacketWelcome welcome(player.first);
                            network.sendPacket(welcome, player.second);
                            #endif
                        }
                    });
                } else {
                    network.registerNetHandler(network::WELCOME, std::make_unique<WelcomeHandler>(componentManager, entityManager));


                network.addHandler(network::PLAYER_SHOOT, [&entityManager, &componentManager]
                    (std::unique_ptr<network::IPacket> packet,
                    std::shared_ptr<asio::ip::tcp::socket> socket) {
                        auto* packetPlayerShoot = dynamic_cast<network::PacketPlayerShoot*>(packet.get());

#ifdef RTYPE_IS_CLIENT
                        if (packetPlayerShoot) {
                            spdlog::info(packetPlayerShoot->netId);
                            for (auto &entity: entityManager.getEntities()) {
                                auto netId = componentManager.getComponent<components::NetId>(entity);
                                auto playerPos = componentManager.getComponent<components::Position>(entity);

                                if (netId && playerPos) {
                                    if (netId->id == packetPlayerShoot->netId) {
                                        size_t projectileId = entityManager.createEntity();

                                        components::Velocity vel = {2.0, 0.0, 0.0};
                                        components::Position pos = {playerPos->x + 10.0f, playerPos->y, playerPos->z};
                                        componentManager.addComponent<components::Velocity>(projectileId, vel);
                                        componentManager.addComponent<components::Position>(projectileId, pos);
                                        componentManager.addComponent<components::Size>(projectileId, {10.0f, 10.0f});
                                        componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}});
                                        componentManager.addComponent<components::Speed>(projectileId, {250});
                                        componentManager.addComponent<components::Damage>(projectileId, {20});
                                        componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true});
                                        components::Sprite projectileSprite = {
                                            pos,
                                            {82.0f, 18.0f},
                                            "assets/sprites/projectile/player-shots.gif",
                                            {1},
                                            {1.0, 1.0},
                                            std::make_shared<sf::Texture>(),
                                            std::make_shared<sf::Sprite>()
                                        };
                                        projectileSprite.texture->loadFromFile(projectileSprite.path);
                                        projectileSprite.sprite->setTexture(*projectileSprite.texture);
                                        sf::IntRect textureRect(82, 165, 82, 18);
                                        projectileSprite.sprite->setTextureRect(textureRect);
                                        projectileSprite.sprite->setPosition({pos.x, pos.y});
                                        components::Animation projAnim = {
                                            "assets/sprite/projectile/player-shots.gif",
                                                2,
                                                10
                                            };
                                        componentManager.addComponent<components::Animation>(projectileId, projAnim);
                                        componentManager.addComponent<components::Sprite>(projectileId, projectileSprite);
                                        components::Projectile projectile = {
                                            pos,
                                            vel,
                                            projAnim,
                                            projectileSprite
                                        };
                                        componentManager.addComponent<components::Projectile>(projectileId, projectile);
                                    }
                                }
                            }
                        }
#endif
                    });

                    network.addHandler(network::PLAYER_COUNT, [&entityManager, &componentManager]
                    (std::unique_ptr<network::IPacket> packet,
                    std::shared_ptr<asio::ip::tcp::socket> socket) {
                        auto* packetPlayerCounter = dynamic_cast<network::PacketPlayerCounter*>(packet.get());

                        if (packetPlayerCounter) {
                            int count = 0;

                            for (auto &entity: entityManager.getEntities()) {
                                auto gameState = componentManager.getComponent<components::GameState>(entity);
                                auto playerCount = componentManager.getComponent<components::Counter>(entity);
                                if (gameState && gameState->isStarted) {
                                    return;
                                }
                                if (playerCount && playerCount->name == "players") {
                                    playerCount->update(packetPlayerCounter->_count);
                                    componentManager.addComponent<components::Counter>(entity, *playerCount);
                                }
                            }
                        }
                    });
                }

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
                        auto r = componentManager.getComponent<components::Running>(tcp);
                        r->running = false;

                        componentManager.addComponent<components::Running>(tcp, *r);
                    }
                }
            }
        }
    }
}
