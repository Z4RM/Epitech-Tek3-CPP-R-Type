/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player service
*/

#pragma once

#include "RType/Entities/Player.hpp"

#define PLAYER_SPEED 100

namespace rtype::services {
    class PlayerService {
    public:

#ifdef RTYPE_IS_CLIENT
        static void createPlayer(
                int netId,
                ecs::EntityManager &entityManager,
                ecs::ComponentManager &componentManager,
                bool actualPlayer = false
        );
#else

        static void createPlayer(
                int netId,
                ecs::EntityManager &entityManager,
                ecs::ComponentManager &componentManager,
                std::shared_ptr<asio::ip::tcp::socket> socket
        );
#endif
    };
}