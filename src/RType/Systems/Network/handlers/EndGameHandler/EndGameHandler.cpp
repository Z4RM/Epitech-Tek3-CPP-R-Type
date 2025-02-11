/*
** EPITECH PROJECT, 2025
** RType
** File description:
** EndGameHandler.cpp
*/


#include "EndGameHandler.hpp"

#include <RType/Components/Shared/IA.hpp>

#include "Network/Packets/Descriptors/PacketEndGame/PacketEndGame.hpp"
#include "ECS/Scene/SceneManager.hpp"
#include "ECS.hpp"
#include "RType/Components/Shared/ActualPlayer.hpp"
#include "RType/Components/Shared/Counter.hpp"
#include "RType/Components/Shared/GameState.hpp"
#include "RType/Components/Shared/Network.hpp"
#include "RType/Levels/LevelManager.hpp"
#include "RType/Entities/Window.hpp"

namespace rtype::systems {
    void EndGameHandler::handle(std::unique_ptr<network::IPacket> packet, std::shared_ptr<asio::ip::tcp::socket> socket) {
        auto* packetEndGame = dynamic_cast<network::PacketEndGame*>(packet.get());

        if (packetEndGame) {
            levels::LevelManager::getInstance().reset();

            for (auto &entity : _entityManager.getEntities()) {
                auto ai = _componentManager.getComponent<components::IA>(entity);
                auto actualPlayer = _componentManager.getComponent<components::ActualPlayer>(entity);
                auto gameState = _componentManager.getComponent<components::GameState>(entity);
                auto playerCount = _componentManager.getComponent<components::Counter>(entity);
                auto window = _componentManager.getComponent<entities::RWindow>(entity);
                auto run = _componentManager.getComponent<components::Running>(entity);

                if (run)
                    continue;

                if (gameState) {
                    gameState->isStarted = false;
                    gameState->playerCount = 0;
                    _componentManager.addComponent<components::GameState>(entity, *gameState, _entityManager);
                    continue;
                }

                if (window)
                    continue;

                _entityManager.destroyEntity(entity);
                _componentManager.removeAllComponent(entity);
            }
            if (packetEndGame->isLose == true) {
                ecs::SceneManager::getInstance().changeScene(2, true);
            } else {
                ecs::SceneManager::getInstance().changeScene(3, true);
            }
        }
    }
}
