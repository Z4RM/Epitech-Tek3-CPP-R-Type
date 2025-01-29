/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player service
*/

#include "PlayerService.hpp"
#include "spdlog/spdlog.h"

namespace rtype::services {
    void PlayerService::createPlayer(int netId, ecs::EntityManager &entityManager, ecs::ComponentManager
    &componentManager, bool actualPlayer) {
    #ifdef RTYPE_IS_CLIENT
        components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};
        entities::Player player2(
                entityManager,
                componentManager,
                {0, 200, 0},
                {0, 0, 0},
                {64, 64},
                sprite2,
                {"", 0, 0},
                { netId },
                {actualPlayer}
        );
    #endif
    }

    void PlayerService::createPlayer(int netId, ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, std::shared_ptr<asio::ip::tcp::socket> socket) {
    #ifndef RTYPE_IS_CLIENT
        rtype::entities::Player playerShip(
            entityManager,
            componentManager,
            {0, 0, 0},
            {0, 0, 0},
            {64, 64},
            { socket },
            { netId },
            {PLAYER_SPEED}
        );
    #endif
    }
}