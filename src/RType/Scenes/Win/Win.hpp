/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Win.hpp
*/


#pragma once
#include "ECS/Scene/AScene.hpp"

namespace rtype::scenes {
    class Win final : public ecs::AScene {
    public:
        Win(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) : AScene(entityManager, componentManager) {};
        ~Win() override = default;

        void load() override;
    };
}
