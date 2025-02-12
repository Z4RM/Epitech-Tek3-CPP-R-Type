/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Bonus component
*/

#pragma once
#include "RType/Models/EBonusType.hpp"

#ifdef RTYPE_IS_CLIENT
namespace rtype::components {
    struct Bonus {
        models::EBonusType type;
    };
}
#else
namespace rtype::components {
    struct Bonus {
        models::EBonusType type;
    };
}
#endif
