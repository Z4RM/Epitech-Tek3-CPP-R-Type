/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player counter
*/

#pragma once
#include "ECS/AEntity.hpp"

namespace rtype::entities {
    class PlayerCounter final: public ecs::AEntity {
        PlayerCounter(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager);
    };
}
