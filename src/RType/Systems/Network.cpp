/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TODO: add description
*/

#include "Network.hpp"

#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "Network/UDPNetwork/UDPNetwork.hpp"
#include "RType/Config/Config.hpp"

namespace rtype::systems {
    void Network::udpProcess(const ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
        static network::UDPNetwork network(Config::getInstance().getNetwork().server.port);

        if (!network.getStarted()) {
            try {
                network.start();
                if (IS_SERVER) {
                    network.addHandler(network::CONNECT, [](std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint
                    endpoint) {
                        network::PacketConnect response;
                        network.sendPacket(response, endpoint);
                    });
                }
            } catch (std::exception &e) {
                spdlog::error("Error while starting udp");
            }
        }
    }

    void Network::tcpProcess(const ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
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
