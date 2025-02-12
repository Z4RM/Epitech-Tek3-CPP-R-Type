/*
** EPITECH PROJECT, 2025
** RType
** File description:
**  Player shoot handler
*/

#include "PlayerShootHandler.hpp"

#include <spdlog/spdlog.h>

#include "Network/Packets/Descriptors/PacketPlayerShoot/PacketPlayerShoot.hpp"
#include "RType/Components/Shared/Network.hpp"
#include "Components.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/ModeManager/ModeManager.hpp"
#include "RType/Services/ProjectileService/ProjectileService.hpp"

namespace rtype::systems {
    void PlayerShootHandler::handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint) {
        auto* packetPlayerShoot = dynamic_cast<network::PacketPlayerShoot*>(packet.get());

        if (packetPlayerShoot) {
            bool gameStarted = false;
            for (auto &entity: _entityManager.getEntities()) {
                auto gameState = _componentManager.getComponent<components::GameState>(entity);

                if (gameState) {
                    gameStarted = true;
                }
            }
            if (!gameStarted)
                return;
            for (auto &entity : _entityManager.getEntities()) {
                const auto event = _componentManager.getComponent<components::EventId>(entity);
                const auto pos = _componentManager.getComponent<components::Position>(entity);

                if (event && event->value == packetPlayerShoot->eventId && event->netIdEmitter == packetPlayerShoot->netId) {
                    if (!IS_SERVER) {
                        _componentManager.addComponent<components::Position>(entity, packetPlayerShoot->pos, _entityManager);
                    }
                    return;
                }
            }
            if (packetPlayerShoot->isPlayer) {
                services::ProjectileService::createProjectile(_entityManager, _componentManager, std::make_shared<components::Position>(packetPlayerShoot->pos),
                packetPlayerShoot->isSuperProjectile, {packetPlayerShoot->eventId, packetPlayerShoot->netId});
            } else {
                services::ProjectileService::createEnemyProjectile(_entityManager, _componentManager, std::make_shared<components::Position>
                (packetPlayerShoot->pos), {packetPlayerShoot->eventId, packetPlayerShoot->netId});
            }
        }
    }
}
