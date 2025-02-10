/*
** EPITECH PROJECT, 2025
** RType
** File description:
**  LevelsRegistered handler
*/

#include "LevelsRegisteredHandler.hpp"

#include <spdlog/spdlog.h>

#include "Network/Packets/Descriptors/PacketLevelsRegistered/PacketLevelsRegistered.hpp"
#include "RType/Components/Shared/Counter.hpp"
#include "RType/Levels/LevelBuilder.hpp"
#include "RType/Levels/LevelManager.hpp"

namespace rtype::systems {
    void LevelsRegisteredHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto* packetLevelsRegistered = dynamic_cast<network::PacketLevelsRegistered*>(packet.get());
        int lowestLevel = 2147483647;

        if (packetLevelsRegistered) {
            for (int level: packetLevelsRegistered->levels) {
                levels::LevelManager::getInstance().registerLevel(std::make_shared<levels::Level>(
                    levels::LevelBuilder()
                    .setNumber(level)
                    .build()
                ));
                if (level < lowestLevel) {
                    lowestLevel = level;
                }
            }

            for (auto &entity : _entityManager.getEntities()) {
                auto counter = _componentManager.getComponent<components::Counter>(entity);

                if (counter && counter->name == "level") {
                    counter->update(lowestLevel);
                    _componentManager.addComponent<components::Counter>(entity, *counter);
                    return;
                }
            }
        }
    }

}
