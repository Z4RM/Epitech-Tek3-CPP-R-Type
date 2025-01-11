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

#ifndef RTYPE_IS_SERVER
#include "RType/Components/Client/Sprite.hpp"
#endif

#include "RType/Config/Config.hpp"
#include "Components.hpp"
#include "Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp"
#include "RType/Entities/Enemy.hpp"
#include "RType/Entities/Player.hpp"

namespace rtype::systems {
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
            std::vector<models::PlayerData> data;

            for (const auto& entity : entityManager.getEntities()) {
                const auto netId = componentManager.getComponent<components::NetId>(entity);
                const auto vel = componentManager.getComponent<components::Velocity>(entity);
                const auto pos = componentManager.getComponent<components::Position>(entity);
                const auto size = componentManager.getComponent<components::Size>(entity);

                if (vel && pos && size && netId) {
                    models::PlayerData pdata{*pos, *vel, *size, *netId};

                    if (netId->id == 0 && !IS_SERVER)
                        data.emplace_back(pdata);

                    if (IS_SERVER && netId->id != 0) {
                        data.emplace_back(pdata);
                    }
                }
            }

            if (!data.empty()) {
                network::PacketPlayersData playersData(data);

                if (!IS_SERVER)
                    network.sendPacket(playersData, network.getServerEndpoint());
                else {
                    for (const auto &entity : entityManager.getEntities()) {
                        const auto net = componentManager.getComponent<components::NetworkConnection>(entity);

                        if (net) {
                            network.sendPacket(playersData, net->endpoint);
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
                static int playerId = 0;

                /** UPDATING SERVER PLAYER LIST **/
                if (playersData) {
                    for (auto &data : playersData->datas) {
                        bool created = false;
                        for (const auto &entity : entityManager.getEntities()) {
                            auto netCo = componentManager.getComponent<components::NetworkConnection>(entity);
                            auto pos = componentManager.getComponent<components::Position>(entity);
                            auto vel = componentManager.getComponent<components::Velocity>(entity);
                            auto size = componentManager.getComponent<components::Size>(entity);
                            auto net = componentManager.getComponent<components::NetId>(entity);

                            if (pos && vel && size && net && netCo && netCo->endpoint == endpoint) {
                                *pos = data.pos;
                                //*vel = data.vel;
                                *size = data.size;
                                created = true;
                            }
                        }
                        if (!created) {
                            playerId++;
                            spdlog::info("New player with id: {}", playerId);
                        #ifndef RTYPE_IS_CLIENT
                                rtype::entities::Player player(
                                entityManager,
                                componentManager,
                                {0, 0, 0},
                                {0, 0, 0},
                                {64, 64},
                                {endpoint},
                                { playerId });
                        #endif
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
                        for (const auto &data: playersData->datas) {
                            bool created = false;

                            for (const auto &entity : entityManager.getEntities()) {
                                auto net = componentManager.getComponent<components::NetId>(entity);

                                if (net) {
                                    int netId = net->id;

                                    if (data.netId.id == netId && netId != 0) {
                                        created = true;
                                        *componentManager.getComponent<components::Position>(entity) = data.pos;
                                        break;
                                    }
                                }
                            }
                            if (!created) {
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
                    } else {
                        spdlog::error("Invalid packet players data received");
                    }
            });
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
