/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TODO: add description
*/

#include "Network.hpp"

#include "Network/Packets/Descriptors/PacketNewPlayer/PacketNewPlayer.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "Network/UDPNetwork/UDPNetwork.hpp"
#include "RType/Components/Client/Sprite.hpp"
#include "RType/Config/Config.hpp"
#include "Components.hpp"
#include "Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp"
#include "RType/Entities/Enemy.hpp"
#include "RType/Entities/Player.hpp"

namespace rtype::systems {
    std::list<std::pair<int, asio::ip::udp::endpoint>> Network::_playerList;

    void Network::udpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        static network::UDPNetwork network(Config::getInstance().getNetwork().server.port);

        if (!network.getStarted()) {
            try {
                addUdpHandlers(network, entityManager, componentManager);
                    auto timer = std::make_shared<asio::steady_timer>(network.getIoContext());
                    schedulePacketSending(entityManager, componentManager, network, timer);
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Error while starting udp");
            }
        }
    }

    void Network::schedulePacketSending(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
        network::UDPNetwork &network, std::shared_ptr<asio::steady_timer> timer) {
        try {
            std::vector<PlayerData> data;

            for (const auto& entity : entityManager.getEntities()) {
                const auto net = componentManager.getComponent<components::Network>(entity);
                const auto vel = componentManager.getComponent<components::Velocity>(entity);
                const auto pos = componentManager.getComponent<components::Position>(entity);
                const auto size = componentManager.getComponent<components::Size>(entity);

                if (net && vel && pos && size) {
                    PlayerData pdata{*pos, *vel, *size, *net};

                    if (net->netId == 0 && !IS_SERVER)
                        data.emplace_back(pdata);

                    if (IS_SERVER && net->netId != 0) {
                        data.emplace_back(pdata);
                    }
                }
            }

            if (!data.empty()) {
                network::PacketPlayersData playersData(data);

                if (!IS_SERVER)
                    network.sendPacket(playersData, network.getServerEndpoint());
                else {
                    for (const auto &player : _playerList) {
                        network.sendPacket(playersData, player.second);
                    }
                }
            }

            timer->expires_after(std::chrono::microseconds(1));
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
            network.addHandler(network::CONNECT, [&network, &entityManager, &componentManager](std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) {

                int playerId = 1;
                network::PacketWelcome response;

                if (!_playerList.empty()) {
                    playerId = _playerList.back().first;
                    playerId++;
                }

                for (const auto& player : _playerList) {
                    network::PacketNewPlayer newPlayer(playerId);
                    network::PacketNewPlayer oldPlayer(player.first);

                    network.sendPacket(newPlayer, player.second);
                    network.sendPacket(oldPlayer, endpoint);
                }

                //TODO: fix this problem, should we use ifndef or the macro is_server
                #ifndef RTYPE_IS_CLIENT
                    rtype::entities::Player player {
                        entityManager,
                        componentManager,
                        {0, 0, 0},
                        {0, 0, 0},
                        {64, 64},
                        {playerId}
                    };
                #endif

                _playerList.emplace_back(playerId, endpoint);
                network.sendPacket(response, endpoint);
            });

            network.addHandler(network::PLAYERS_DATA, [&network, &entityManager, &componentManager](std::unique_ptr<network::IPacket>
            packet, asio::ip::udp::endpoint endpoint) {
                auto* playersData = dynamic_cast<network::PacketPlayersData*>(packet.get());

                /** UPDATING SERVER PLAYER LIST **/
                if (playersData) {
                    for (const auto &data: playersData->datas) {
                        for (const auto &player : _playerList) {
                            if (player.second == endpoint) {
                                for (const auto &entity : entityManager.getEntities()) {
                                    auto pos = componentManager.getComponent<components::Position>(entity);
                                    auto vel = componentManager.getComponent<components::Velocity>(entity);
                                    auto size = componentManager.getComponent<components::Size>(entity);
                                    auto net = componentManager.getComponent<components::Network>(entity);

                                    if (pos && vel && size && net && net->netId == player.first) {
                                        *pos = data.pos;
                                        //*vel = data.vel;
                                        *size = data.size;
                                    }

                                }
                            }
                        }
                    }
                } else {
                    spdlog::error("Invalid packet players data received");
                }
            });

        } else {

            network.addHandler(network::WELCOME, [](std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) {
                spdlog::info("Server said welcome : successfully connected to the UDP game");
            });

            network.addHandler(network::PLAYERS_DATA, [&network, &entityManager, &componentManager](std::unique_ptr<network::IPacket>
                packet, asio::ip::udp::endpoint endpoint) {
                    auto* playersData = dynamic_cast<network::PacketPlayersData*>(packet.get());

                    if (playersData) {
                        for (const auto &entity : entityManager.getEntities()) {
                            auto net = componentManager.getComponent<components::Network>(entity);

                            if (net) {
                                int netId = net->netId;
                                for (const auto &data: playersData->datas) {
                                    if (data.network.netId == netId && netId != 0) {
                                        *componentManager.getComponent<components::Position>(entity) = data.pos;
                                        //*componentManager.getComponent<components::Velocity>(entity) = data.vel;
                                        break;
                                    }
                                }
                            }
                        }
                    } else {
                        spdlog::error("Invalid packet players data received");
                    }
            });

        #ifndef RTYPE_IS_SERVER

            network.addHandler(network::NEW_PLAYER, [&entityManager, &componentManager](std::unique_ptr<network::IPacket> packet,
            asio::ip::udp::endpoint
            endpoint) {
                auto* packetNewPlayer = dynamic_cast<network::PacketNewPlayer*>(packet.get());

                if (packetNewPlayer) {
                    int id = packetNewPlayer->id;

                    components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};
                    entities::Player player2(
                            entityManager,
                            componentManager,
                            {0, 200, 0},
                            {0, 0, 0},
                            {64, 64},
                            sprite2,
                            {"", 0, 0},
                            { id }
                    );
                    spdlog::info("New player with id: {}", std::to_string(id));
                } else {
                    spdlog::error("Bad packet NEW_PLAYER received");
                }
            });

        #endif
        }
    }


    void Network::tcpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        static network::TCPNetwork network(Config::getInstance().getNetwork().server.port);

        if (!network.getStarted()) {
            try {
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Error while starting tcp");
            }
        }
    }

}
