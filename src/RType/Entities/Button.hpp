/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Button entities
*/

#pragma once
#include "ECS/AEntity.hpp"
#include "RType/Components/Client/OnClick.hpp"
#include "RType/Components/Client/SfText.hpp"

namespace rtype::entities {
    class Button final : public ecs::AEntity {
    public:
        Button(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::OnClick,
        components::SfText text);
    };
}
