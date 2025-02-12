/*
** EPITECH PROJECT, 2025
** RType
** File description:
** player counter
*/

#include "PlayerCounter.hpp"
#include "RType/Components/Shared/Counter.hpp"

namespace rtype::entities {
#ifdef RTYPE_IS_CLIENT
    PlayerCounter::PlayerCounter(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, sf::Vector2f pos) : AEntity
    (entityManager) {
        std::string counterName = "players";
        components::Counter count(0, 4, counterName, pos, 35);
        componentManager.addComponent<components::Counter>(_id, count, entityManager);
    }
#else
    PlayerCounter::PlayerCounter(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : AEntity
(entityManager) {
        std::string counterName = "players";
        components::Counter count(0, 4, counterName);
        componentManager.addComponent<components::Counter>(_id, count, entityManager);
    }
#endif
}
