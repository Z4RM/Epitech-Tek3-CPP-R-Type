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
        //SHARED
        CONNECT = 100,
        WELCOME = 101,
        PLAYER_COUNT = 102,

        NEW_PLAYER = 200,
        PLAYERS_DATA = 201,
        PLAYER_SHOOT = 202,

        ENEMIES_DATA = 301,

        START_GAME = 400
    };
}