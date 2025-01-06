/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Movement.cpp
*/

#include "Movement.hpp"
#include "Components.hpp"
#include <iostream>

void rtype::systems::Movement::move(const rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager)
{
    for (auto& entity : entityManager.getEntities()) {
        const auto pos = componentManager.getComponent<components::Position>(entity);
        const auto vel = componentManager.getComponent<components::Velocity>(entity);
        if (pos && vel)
        {
            pos->x += vel->x;
            pos->y += vel->y;
            pos->z += vel->z;
        }
        const auto ia = componentManager.getComponent<components::IA>(entity);
        const auto pos2 = componentManager.getComponent<components::Position>(entity);
        if (ia && pos2) {
            const auto move = ia->moves.begin();
            const components::Velocity velTarget = move->second;
            pos2->x += velTarget.x;
            pos2->y += velTarget.y;
            pos2->z += velTarget.z;
        }
    }
}
