/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Win.hpp
*/


#pragma once
#include "ECS/Scene/AScene.hpp"

namespace rtype::scenes {
    class Lose final : public ecs::AScene {
    public:
        Lose(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) : AScene(entityManager, componentManager) {};
        ~Lose() override = default;

        void load() override;
    };
}
