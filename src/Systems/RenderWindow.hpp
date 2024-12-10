/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RenderWindow.hpp
*/

#pragma once
#include "ECS.hpp"
#include "Structures.hpp"
#include "RType/Components/Window.hpp"
#include "Systems/InputSystem.hpp"

namespace rtype::systems
{
    class RenderWindowSys {
    public:

        static void render(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);
        static void createWindow(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);
    };
}
