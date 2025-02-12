/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ProjectileData
*/

#pragma once
#include "RType/Components/Shared/EventId.hpp"
#include "RType/Components/Shared/ProjectileInfo.hpp"

namespace rtype::models {
    struct ProjectileData {
        components::EventId event;
        components::ProjectileInfo projectileInfo;
        components::Position pos;
    };
}
