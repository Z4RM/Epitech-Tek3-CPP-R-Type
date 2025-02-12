/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Bonus Handler
*/

#include "BonusHandler.hpp"

#include <spdlog/spdlog.h>

#include "Network/Packets/Descriptors/PacketBonus/PacketBonus.hpp"
#include "RType/Components/Shared/EventId.hpp"
#include "RType/Services/BonusService/BonusService.hpp"

namespace rtype::systems {
    void BonusHandler::handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) {
        auto* packetBonus = dynamic_cast<network::PacketBonus*>(packet.get());

        if (packetBonus) {
            auto type = static_cast<models::EBonusType>(packetBonus->type);
            for (auto &entity: _entityManager.getEntities()) {
                auto event = _componentManager.getComponent<components::EventId>(entity);

                if (event && event->value == packetBonus->eventId && packetBonus->netIdEmitter == event->netIdEmitter)
                    return;
            }
            services::BonusService::createBonus(_entityManager, _componentManager, type, packetBonus->pos, packetBonus->eventId);
        }
    }
}
