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
#include "RType/Entities/Enemy.hpp"
#include "RType/Entities/Player.hpp"
#include "RType/Entities/Window.hpp"

namespace rtype::systems {
    std::list<std::pair<int, asio::ip::udp::endpoint>> Network::_playerList;

    void Network::udpProcess(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        static network::UDPNetwork network(Config::getInstance().getNetwork().server.port);

        if (!network.getStarted()) {
            try {
                addUdpHandlers(network, entityManager, componentManager);
                network.start();
            } catch (std::exception &e) {
                spdlog::error("Error while starting udp");
            }
        }
    }

    void Network::addUdpHandlers(network::UDPNetwork &network, ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        if (IS_SERVER) {

            network.addHandler(network::CONNECT, [&network](std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) {
                network::PacketWelcome response;

                for (auto player : _playerList) {
                    network::PacketNewPlayer newPlayer;

                    network.sendPacket(newPlayer, player.second);
                    network.sendPacket(newPlayer, endpoint);
                }

                if (_playerList.empty()) {
                    _playerList.emplace_back(1, endpoint);
                } else {
                    int last_id = _playerList.back().first;

                    last_id++;
                    _playerList.emplace_back(last_id, endpoint);
                }
                network.sendPacket(response, endpoint);
            });

        } else {

            network.addHandler(network::WELCOME, [](std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) {
                network::PacketWelcome response;

                spdlog::info("Server said welcome : successfully connected to the UDP game");
            });

        #ifndef RTYPE_IS_SERVER

            network.addHandler(network::NEW_PLAYER, [&entityManager, &componentManager](std::unique_ptr<network::IPacket> packet,
            asio::ip::udp::endpoint
            endpoint) {
                components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};
                entities::Player player2(
                        entityManager,
                        componentManager,
                        {0, 200, 0},
                        {0, 0, 0},
                        {64, 64},
                        sprite2,
                        {"", 0, 0}
                );
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
