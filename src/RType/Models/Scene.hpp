/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Scene
*/

#pragma once
#include <functional>

#include "ECS/ComponentManager.hpp"
#include "ECS/EntityManager.hpp"

namespace rtype::models {
    struct Scene {
        std::function<void()> load;
        ecs::EntityManager &entityManager;
        ecs::ComponentManager &componentManager;
    };
}
