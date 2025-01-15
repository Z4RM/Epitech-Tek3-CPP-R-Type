/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Image entity header
*/

#pragma once
#include "ECS/ComponentManager.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/AEntity.hpp"
#include "RType/Components/Client/Sprite.hpp"

namespace rtype::entities {
    class Image : public ecs::AEntity {
    public:
        Image(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Sprite sprite, bool isBg
        = false);
    };
}
