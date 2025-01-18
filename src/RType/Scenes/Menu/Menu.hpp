/*
** EPITECH PROJECT, 2025
** RType
** File description:
**
*/

#pragma once
#include "ECS/Scene/AScene.hpp"

namespace rtype::scenes {
    class Menu final : public ecs::AScene {
    public:

        Menu(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) : AScene(entityManager, componentManager) {};
        ~Menu() override = default;

        void load() override;
    };
}
