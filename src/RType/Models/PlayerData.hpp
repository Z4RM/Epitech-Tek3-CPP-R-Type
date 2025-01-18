/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PlayerData.hpp
*/

#pragma once

#include <optional>
#include "Components.hpp"

namespace rtype::models {
    /** Represent the datas of a player **/
    struct PlayerData {
        components::Position pos{};
        components::Velocity vel{};
        components::Size size{};
        components::NetId netId;
        int health = 0;
        std::optional<components::NetworkConnection> network;
    };
}
