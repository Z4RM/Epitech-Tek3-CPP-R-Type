/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Connect Handler
*/

#include "ConnectHandler.hpp"

#include "RType/Systems/Network/Network.hpp"
#include "Components.hpp"
#include "Network/Packets/Descriptors/PacketLevelsRegistered/PacketLevelsRegistered.hpp"
#include "Network/Packets/Descriptors/PacketPlayerCounter/PacketPlayerCounter.hpp"
#include "RType/Levels/LevelManager.hpp"
#include "RType/Services/PlayerService/PlayerService.hpp"
#include "spdlog/spdlog.h"

namespace rtype::systems {
    void ConnectHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        components::MenuState newState = { 0 };
        unsigned int menuStateEntity = -1;

        for (auto &entity: _entityManager.getEntities()) {
            auto menuState = _componentManager.getComponent<components::MenuState>(entity);

            if (menuState) {
                newState = *menuState;
                menuStateEntity = entity;
            }
        }
        if (menuStateEntity < 0)
            return;

        if (newState.playerCount < 4) {
            newState.playerCount += 1;
            network::PacketPlayerCounter packetCount(newState.playerCount);
            std::vector<int> levels;

            for (auto &level : levels::LevelManager::getInstance().getLevels()) {
                levels.emplace_back(level->getNumber());
            }

            network::PacketLevelsRegistered packetLevelsRegistered(levels);

#ifdef RTYPE_IS_SERVER
            services::PlayerService::createPlayer(_entityManager, _componentManager, socket);
#endif

            for (auto &entity : _entityManager.getEntities()) {
                auto netCo = _componentManager.getComponent<components::NetworkConnection>(entity);

                if (netCo) {
                    network::TCPNetwork::getInstance().sendPacket(packetCount, netCo->socket);
                    network::TCPNetwork::getInstance().sendPacket(packetLevelsRegistered, netCo->socket);
                }
            }
            _componentManager.addComponent<components::MenuState>(menuStateEntity, newState);
            spdlog::info("New player created with network ID {}", Network::globalNetId.load());
        } else {
            //todo: send packet game already started to the client
        }
    }
}
