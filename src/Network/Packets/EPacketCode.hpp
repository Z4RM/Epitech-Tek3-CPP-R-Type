/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Enum for packets code
*/

#pragma once

namespace rtype::network {
    /** @brief Enum for packet codes **/
    enum EPacketCode {
        CONNECT = 100,
        WELCOME = 101,
        PLAYER_COUNT = 102,
        LEVELS_REGISTERED = 103,

        PLAYERS_DATA = 201,
        PLAYER_SHOOT = 202,
        BONUS_SPAWN = 203,

        ENEMIES_DATA = 301,

        START_GAME = 400,
        END_GAME = 401
    };
}