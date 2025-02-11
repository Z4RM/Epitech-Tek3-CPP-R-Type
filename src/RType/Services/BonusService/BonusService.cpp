/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Bonus Service
*/

#include "BonusService.hpp"

#include <RType/Components/Shared/Position.hpp>

#include "RType/Components/Shared/Bonus.hpp"
#include <spdlog/spdlog.h>

#include "RType/Components/Shared/EventId.hpp"

namespace rtype::services {
    void BonusService::createBonus(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
    models::EBonusType type, components::Position pos, long eventId) {
        unsigned int entityId = entityManager.createEntity();

        components::Bonus bonus = { type };
        components::EventId event = { eventId };
        componentManager.addComponent<components::Bonus>(entityId, bonus, entityManager);
        componentManager.addComponent<components::Position>(entityId, pos, entityManager);
        componentManager.addComponent<components::EventId>(entityId, event, entityManager);
        spdlog::debug("Spawning a bonus: {} at pos: {}:{}", std::to_string(type), pos.x, pos.y);
    }
}
