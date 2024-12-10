/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.hpp
*/

#ifndef INPUTSYSTEM_HPP
#define INPUTSYSTEM_HPP

#include "ECS.hpp"
#include "RType/Components/InputHandler.hpp"
#include <SFML/Window/Event.hpp>

namespace rtype::systems {
    /**
     * @class InputSystem
     * @brief Handles input events and triggers corresponding actions.
     *
     * The `InputSystem` class is responsible for processing input events, such as
     * key presses, and invoking appropriate actions based on the input. It interacts
     * with the entity and component managers to access the game entities and their
     * associated input handlers.
     */
    class InputSystem {
    public:
        /**
         * @brief Handles the input event and triggers actions for entities.
         *
         * This static function processes input events and triggers the associated
         * actions for entities that have an `InputHandler` component.
         *
         * @param entityManager A reference to the entity manager, used to access
         *        the entities in the game.
         * @param componentManager A reference to the component manager, used to
         *        access the components of the entities.
         * @param event The input event (e.g., keyboard key press) to process.
         */
        static void handleInput(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, const sf::Event &event);
    };
}

#endif //INPUTSYSTEM_HPP
