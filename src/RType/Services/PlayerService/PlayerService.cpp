/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player service
*/

#include "RType/ModeManager/ModeManager.hpp"
#include "PlayerService.hpp"

#include <utility>

#include "RType/Systems/Network/Network.hpp"
#include "spdlog/spdlog.h"

namespace rtype::services {
#ifdef RTYPE_IS_CLIENT

    void PlayerService::createPlayer(
            int netId,
            ecs::EntityManager &entityManager,
            ecs::ComponentManager &componentManager,
            bool actualPlayer
    ) {
        components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};

        entities::Player player2(
                entityManager,
                componentManager,
                {10, 50, 0},
                {0, 0, 0},
                {64, 64},
                sprite2,
                {"", 0, 0},
                {netId},
                {actualPlayer}
        );
    }

#else

    void PlayerService::createPlayer(
            ecs::EntityManager &entityManager,
            ecs::ComponentManager &componentManager,
            std::shared_ptr<asio::ip::tcp::socket> socket
    ) {
            systems::Network::globalNetId.store(systems::Network::globalNetId.load() + 1);
            rtype::entities::Player playerShip(
                    entityManager,
                    componentManager,
                    {10, 50, 0},
                    {0, 0, 0},
                    {64, 64},
                    {std::move(socket)},
                    {systems::Network::globalNetId.load()},
                    {PLAYER_SPEED}
            );
    }
#endif
}
