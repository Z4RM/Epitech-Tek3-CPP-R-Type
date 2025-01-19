/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Player counter
*/

#pragma once

#include "ECS/AEntity.hpp"



#ifdef RTYPE_IS_CLIENT
#include <SFML/System/Vector2.hpp>
#endif

namespace rtype::entities {
    class PlayerCounter final: public ecs::AEntity {
    public:
#ifdef RTYPE_IS_CLIENT
        PlayerCounter(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, sf::Vector2f pos);
#else
        PlayerCounter(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager);
#endif
    };
}
