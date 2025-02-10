/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.cpp
*/

#include "Components.hpp"
#include "InputSystem.hpp"

#include "ECS/Scene/SceneManager.hpp"

void rtype::systems::InputSystem::handleInput(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, const
                                              sf::Event &event, entities::RWindow *window) {
    const auto entities = entityManager.getEntities();
    for (const auto entity : entities) {
        auto inputHandler = componentManager.getComponent<rtype::components::InputHandler>(entity);

        //TODO: onclick should not depend on text
        auto text = componentManager.getComponent<rtype::components::SfText>(entity);
        auto onClick = componentManager.getComponent<rtype::components::OnClick>(entity);
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window->window);
        sf::Vector2f worldPos = window->window->mapPixelToCoords(mousePos);

        if (text) {
            if (text->text.getGlobalBounds().contains(worldPos)) {
                text->text.setFillColor(sf::Color::Red);
                componentManager.addComponent<components::SfText>(entity, *text);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (onClick) {
                        onClick->fn();
                    }
                }
            } else {
                sf::Color color = text->text.getFillColor();
                if (color == sf::Color::Red) {
                    text->text.setFillColor(sf::Color::White);
                    componentManager.addComponent<components::SfText>(entity, *text);
                }
            }
        }

        if (!inputHandler)
            continue;

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            auto [fst, snd] = inputHandler->keyActions.equal_range(event.key.code);

            if (fst == inputHandler->keyActions.end())
                return;

            for (auto todo = fst; todo != snd; ++todo) {
                if (event.type == todo->second.first)
                    todo->second.second();
            }
        }
    }
}
