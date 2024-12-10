/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.cpp
*/

#include "InputSystem.hpp"

/**
 * @brief Processes input events and triggers associated actions for entities.
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
 *        retrieve the components of the entities.
 * @param event The input event (e.g., a key press) to process.
 */
void rtype::systems::InputSystem::handleInput(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, const sf::Event &event) {
    if (event.type != sf::Event::KeyPressed)
        return;

    const auto entities = entityManager.getEntities();
    for (const auto &entity : entities) {
        auto inputHandler = componentManager.getComponent<rtype::components::InputHandler>(entity);
        if (!inputHandler)
            continue;

        auto action = inputHandler->keyActions.find(event.key.code);
        if (action != inputHandler->keyActions.end())
            action->second();
    }
}
