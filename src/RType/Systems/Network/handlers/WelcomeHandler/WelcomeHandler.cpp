/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Welcome packet handler
*/

#include "WelcomeHandler.hpp"

#include "ECS/Scene/SceneManager.hpp"
#include "Network/Packets/Descriptors/PacketWelcome/PacketWelcome.hpp"
#include "RType/Entities/Player.hpp"
#include "spdlog/spdlog.h"

namespace rtype::systems {
    void WelcomeHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto* packetWelcome = dynamic_cast<network::PacketWelcome*>(packet.get());

        if (packetWelcome) {
            spdlog::info("Server said welcome, network ID is: {}", packetWelcome->netId);
            ecs::SceneManager::getInstance().changeScene(1, true);

        //TODO: use a service
        #ifdef RTYPE_IS_CLIENT
            components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};
            entities::Player player2(
                    _entityManager,
                    _componentManager,
                    {0, 200, 0},
                    {0, 0, 0},
                    {64, 64},
                    sprite2,
                    {"", 0, 0},
                    [](int id) {
                        network::PacketPlayerShoot sendPlayerShoot(id);
                        //network::TCPNetwork::getInstance().sendPacket(sendPlayerShoot);
                    },
                    {packetWelcome->netId},
                    {true}
            );
        #endif
        }
    }
}
