/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Start game handler
*/

#include "StartGameHandler.hpp"

#include "Network/Packets/Descriptors/PacketWelcome/PacketWelcome.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/Components/Shared/GameState.hpp"
#include "RType/Components/Shared/NetId.hpp"
#include "RType/Components/Shared/Network.hpp"

namespace rtype::systems {
    void StartGameHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        for (auto &entity: _entityManager.getEntities()) {
            auto gameState = _componentManager.getComponent<components::GameState>(entity);

            if (gameState) {
                if (gameState->isStarted)
                    return;
                gameState->isStarted = true;
                _componentManager.addComponent<components::GameState>(entity, *gameState);
                for (auto &entity: _entityManager.getEntities()) {
                    auto netCo = _componentManager.getComponent<components::NetworkConnection>(entity);
                    auto netId = _componentManager.getComponent<components::NetId>(entity);

                    if (netCo && netId) {
                        network::PacketWelcome welcome(netId->id);
                        network::TCPNetwork::getInstance().sendPacket(welcome, netCo->socket);
                    }
                }
            }
        }
    }
}
