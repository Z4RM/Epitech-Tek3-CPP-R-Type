/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Bonus Service
*/

#pragma once
#include <RType/Components/Shared/Position.hpp>

#include "ECS/ComponentManager.hpp"
#include "ECS/EntityManager.hpp"
#include "RType/Models/EBonusType.hpp"


namespace rtype::services {
    class BonusService {
    public:
        static void createBonus(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, models::EBonusType
        type, components::Position pos, long eventId);
    };
}
