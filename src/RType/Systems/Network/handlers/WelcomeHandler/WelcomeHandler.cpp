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
#include "RType/Services/PlayerService/PlayerService.hpp"
#include "spdlog/spdlog.h"

namespace rtype::systems {
    void WelcomeHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto* packetWelcome = dynamic_cast<network::PacketWelcome*>(packet.get());

        if (packetWelcome) {
            spdlog::info("Server said welcome, network ID is: {}", packetWelcome->netId);
            ecs::SceneManager::getInstance().changeScene(1, true);
            services::PlayerService::createPlayer(packetWelcome->netId, true, _entityManager, _componentManager);
        }
    }
}
