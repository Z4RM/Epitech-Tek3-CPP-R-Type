/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player count packet handler
*/

#include "PlayerCountHandler.hpp"

#include "Network/Packets/Descriptors/PacketPlayerCounter/PacketPlayerCounter.hpp"
#include "Components.hpp"

namespace rtype::systems {
    void PlayerCountHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto* packetPlayerCounter = dynamic_cast<network::PacketPlayerCounter*>(packet.get());

        if (packetPlayerCounter) {
            int count = 0;

            for (auto &entity: _entityManager.getEntities()) {
                auto gameState = _componentManager.getComponent<components::GameState>(entity);
                auto playerCount = _componentManager.getComponent<components::Counter>(entity);
                if (gameState && gameState->isStarted) {
                    return;
                }
                if (playerCount && playerCount->name == "players") {
                    playerCount->update(packetPlayerCounter->count);
                    _componentManager.addComponent<components::Counter>(entity, *playerCount, _entityManager);
                }
            }
        }
    }

}
