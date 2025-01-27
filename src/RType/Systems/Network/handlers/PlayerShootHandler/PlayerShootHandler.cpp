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
#include "RType/Services/ProjectileService/ProjectileService.hpp"

namespace rtype::systems {
    void PlayerShootHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {

    auto* packetPlayerShoot = dynamic_cast<network::PacketPlayerShoot*>(packet.get());

    if (packetPlayerShoot) {
        for (auto &entity: _entityManager.getEntities()) {
            //TODO: use directly health component instead of this useless component
            const auto dead = _componentManager.getComponent<components::Dead>(entity);

            if (dead)
                continue;

                auto netco = _componentManager.getComponent<components::NetworkConnection>(entity);
                auto netId = _componentManager.getComponent<components::NetId>(entity);
                auto playerPos = _componentManager.getComponent<components::Position>(entity);

                //TODO: make a service that create a projectile
                if (netco && netId) {
                    if (netId->id == packetPlayerShoot->netId) {
                        services::ProjectileService::createProjectile(_entityManager, _componentManager, playerPos);
                        network::PacketPlayerShoot newPacket(packetPlayerShoot->netId);

                        for (auto &entity : _entityManager.getEntities()) {
                            auto netco = _componentManager.getComponent<components::NetworkConnection>(entity);
                            auto netId = _componentManager.getComponent<components::NetId>(entity);

                            if (netco && netco->socket != socket) {
                                network::TCPNetwork::getInstance().sendPacket(newPacket, netco->socket);
                            }
                        }
                    }
                }
            }
        }
    }

}
