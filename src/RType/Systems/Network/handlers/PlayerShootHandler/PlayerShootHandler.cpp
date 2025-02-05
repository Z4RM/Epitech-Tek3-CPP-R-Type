/*
** EPITECH PROJECT, 2025
** RType
** File description:
**  Player shoot handler
*/

#include "PlayerShootHandler.hpp"

#include "Network/Packets/Descriptors/PacketPlayerShoot/PacketPlayerShoot.hpp"
#include "RType/Components/Shared/Network.hpp"
#include "Components.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/ModeManager/ModeManager.hpp"
#include "RType/Services/ProjectileService/ProjectileService.hpp"

namespace rtype::systems {
    void PlayerShootHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto* packetPlayerShoot = dynamic_cast<network::PacketPlayerShoot*>(packet.get());

        if (packetPlayerShoot) {
            for (auto &entity: _entityManager.getEntities()) {
                //TODO: use directly health component instead of this useless component
                const auto health = _componentManager.getComponent<components::Health>(entity);

                if (health && health->value <= 0)
                    continue;

                auto netco = _componentManager.getComponent<components::NetworkConnection>(entity);
                auto netId = _componentManager.getComponent<components::NetId>(entity);
                auto playerPos = _componentManager.getComponent<components::Position>(entity);

                if (netId && netId->id == packetPlayerShoot->netId) {
                    services::ProjectileService::createProjectile(_entityManager, _componentManager, playerPos, packetPlayerShoot->isSuperProjectile);
                    if (IS_SERVER) {
                        network::PacketPlayerShoot newPacket(packetPlayerShoot->netId, packetPlayerShoot->isSuperProjectile);
                        for (auto &playerEntity : _entityManager.getEntities()) {
                            auto networkConnection = _componentManager.getComponent<components::NetworkConnection>(playerEntity);
                            auto networkId = _componentManager.getComponent<components::NetId>(playerEntity);

                            if (networkConnection && networkConnection->socket != socket) {
                                network::TCPNetwork::getInstance().sendPacket(newPacket, networkConnection->socket);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}
