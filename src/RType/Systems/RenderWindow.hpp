/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RenderWindow.hpp
*/

#pragma once
#include "ECS.hpp"

/**
 * @namespace rtype::systems
 * @brief Contains system-level classes and functionalities for RType.
 *
 * The `rtype::systems` namespace includes various system implementations
 * that handle specific responsibilities within the ECS framework.
 */
namespace rtype::systems
{
    /**
     * @class RenderWindowSys
     * @brief Manages rendering and window creation in the ECS framework.
     *
     * The `RenderWindowSys` class provides static methods for rendering
     * entities and creating windows. This system is responsible for managing
     * all rendering-related operations in the game engine.
     */
    class RenderWindowSys {
    public:
        /**
         * @brief Renders entities in the ECS system.
         *
         * This method processes and renders all drawable entities managed by
         * the `EntityManager` and their associated components managed by the
         * `ComponentManager`.
         *
         * @param entityManager A reference to the `EntityManager` that holds
         *        all entities in the ECS system.
         * @param componentManager A reference to the `ComponentManager` that
         *        manages drawable and rendering-related components.
         */
        static void render(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);

        /**
         * @brief Creates a new window using the ECS framework.
         *
         * This method initializes a new rendering window by leveraging the
         * `EntityManager` and `ComponentManager`. It uses components to define
         * window properties such as size, title, and style.
         *
         * @param entityManager A constant reference to the `EntityManager` for
         *        accessing entities involved in window creation.
         * @param componentManager A reference to the `ComponentManager` for
         *        managing window-related components.
         */
        static void createWindow(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);

    private:
        static std::vector<rtype::ecs::Entity> _getEntitiesSortedByZIndex(
                const rtype::ecs::EntityManager& entityManager,
                rtype::ecs::ComponentManager& componentManager
        );
    };
}
