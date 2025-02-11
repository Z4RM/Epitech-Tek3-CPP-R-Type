/*
** EPITECH PROJECT, 2025
** RType
** File description:
** actual player bonuses
*/

#pragma once
#include <vector>

#include "RType/Models/EBonusType.hpp"

#ifdef RTYPE_IS_SERVER
namespace rtype::components {
    struct PlayerBonuses {
        std::vector<models::EBonusType> bonuses {};
    };
}
#else
namespace rtype::components {
    struct PlayerBonuses {
        std::map<models::EBonusType, sf::Sprite> bonuses {};
    };
}
#endif
