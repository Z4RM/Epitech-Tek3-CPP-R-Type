/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.hpp
*/

#ifndef INPUTSYSTEM_HPP
#define INPUTSYSTEM_HPP

#include "ECS.hpp"

#include <SFML/Window/Event.hpp>

#include "RType/Entities/Window.hpp"

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
         * This function handles input events, specifically key presses, and invokes
         * the corresponding actions defined in the `InputHandler` components of entities.
         *
         * It iterates through all the entities, checks for an `InputHandler` component,
         * and, if the key pressed corresponds to an action in the component, triggers
         * that action.
         *
         * @param entityManager A reference to the entity manager, used to access
         *        the entities in the game.
         * @param componentManager A reference to the component manager, used to
         *        access the components of the entities.
         * @param event The input event (e.g., keyboard key press) to process.
         */
        static void handleInput(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, const sf::Event
        &event, entities::RWindow *window);
    };
}

#endif //INPUTSYSTEM_HPP
