/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.cpp
*/

#include "InputSystem.hpp"

#include "Components.hpp"

void rtype::systems::InputSystem::handleInput(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, const sf::Event &event) {
    const auto entities = entityManager.getEntities();
    for (const auto &entity : entities) {
        auto inputHandler = componentManager.getComponent<rtype::components::InputHandler>(entity);
        if (!inputHandler)
            continue;

        auto action = inputHandler->keyActions.equal_range(event.key.code);
        for (auto& todo = action.first; todo != action.second; todo++) {
            if (event.type == todo->second.first)
                todo->second.second(componentManager, entity);
        }
    }
}
