/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.cpp
*/

#include "InputSystem.hpp"

#include <spdlog/spdlog.h>

#include "Components.hpp"

void rtype::systems::InputSystem::handleInput(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, const
 sf::Event &event, entities::RWindow *window) {
    const auto entities = entityManager.getEntities();
    for (const auto &entity : entities) {
        auto inputHandler = componentManager.getComponent<rtype::components::InputHandler>(entity);

        //TODO: onclick should not depend on text
        auto text = componentManager.getComponent<rtype::components::SfText>(entity);
        auto onClick = componentManager.getComponent<rtype::components::OnClick>(entity);
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window->window);
        sf::Vector2f worldPos = window->window->mapPixelToCoords(mousePos);

        if (text) {
            if (text->text.getGlobalBounds().contains(worldPos)) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (onClick) {
                        onClick->fn();
                    }
                }
                text->text.setFillColor(sf::Color::Red);
            } else {
                sf::Color color = text->text.getColor();
                if (color == sf::Color::Red)
                    text->text.setFillColor(sf::Color::White);
            }
        }

        if (!inputHandler)
            continue;

        auto action = inputHandler->keyActions.equal_range(event.key.code);
        for (auto& todo = action.first; todo != action.second; todo++) {
            if (event.type == todo->second.first)
                todo->second.second();
        }
    }
}
