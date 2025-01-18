/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Game scene
*/

#pragma once
#include "ECS/Scene/AScene.hpp"

namespace rtype::scenes {
    class Game final : public ecs::AScene {
    public:
        Game(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) : AScene(entityManager, componentManager) {}

        void load() override;
    };
}
