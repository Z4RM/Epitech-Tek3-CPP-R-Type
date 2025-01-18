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

        NEW_PLAYER = 200,
        PLAYERS_DATA = 201,

        ENEMIES_DATA = 301,

        START_GAME = 400
    };
}