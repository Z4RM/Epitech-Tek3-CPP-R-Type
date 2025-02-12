/*
** EPITECH PROJECT, 2025
** RType
** File description:
** BonusData
*/

#pragma once
#include <RType/Components/Shared/Position.hpp>

#include "EBonusType.hpp"
#include "RType/Components/Shared/EventId.hpp"

namespace rtype::models {
    struct BonusData {
        components::Position pos = {};
        int eventId = 0;
        EBonusType bonusType = EBonusType::FORCE;
    };
}
