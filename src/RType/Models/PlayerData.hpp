/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "Components.hpp"

namespace rtype {
    struct PlayerData {
        components::Position pos{};
        components::Velocity vel{};
        components::Size size{};
        components::Network network;

    };
}
