/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RenderWindow.cpp
*/

#include "RenderWindow.hpp"

#include <iostream>

#include "InputSystem.hpp"
#include "Components.hpp"
#include "RType/Entities/Window.hpp"

std::vector<rtype::ecs::Entity> getEntitiesSortedByZIndex(
    const rtype::ecs::EntityManager& entityManager,
    rtype::ecs::ComponentManager& componentManager
) {
    const auto entities = entityManager.getEntities();
    std::vector<rtype::ecs::Entity> renderableEntities;

    for (auto entity : entities) {
        if (componentManager.getComponent<rtype::components::Sprite>(entity)) {
            renderableEntities.push_back({entity});
        }
    }

    std::sort(renderableEntities.begin(), renderableEntities.end(),
              [&componentManager](const rtype::ecs::Entity a, const rtype::ecs::Entity b) {
                  auto zIndexA = componentManager.getComponent<rtype::components::Sprite>(a.id)->priority.value;
                  auto zIndexB = componentManager.getComponent<rtype::components::Sprite>(b.id)->priority.value;
                  return zIndexA < zIndexB;
              });

    return renderableEntities;
}

void rtype::systems::RenderWindowSys::render(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager)
{
    for (const auto& entity : entityManager.getEntities()) {
        const auto renderWindow = componentManager.getComponent<rtype::components::RWindow>(entity);

        if (!renderWindow)
            continue;
        sf::Event event{};
        while (renderWindow->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderWindow->window->close();
                return;
            }
            rtype::systems::InputSystem::handleInput(entityManager, componentManager, event);
        }
        auto sortedEntities = getEntitiesSortedByZIndex(entityManager, componentManager);

        for (auto e : sortedEntities) {
            auto sprite = componentManager.getComponent<components::Sprite>(e.id);
            if (sprite && sprite->sprite) {
                auto pos = componentManager.getComponent<components::Position>(e.id);
                if (pos)
                    sprite->sprite->setPosition({pos->x, pos->y});
                renderWindow->window->draw(*sprite->sprite);
            }
        }
        renderWindow->window->display();
    }
}

void rtype::systems::RenderWindowSys::createWindow(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager)
{
    for (const auto& entity : entityManager.getEntities()) {
        const auto renderWindow = componentManager.getComponent<rtype::components::RWindow>(entity);
        const auto mode = componentManager.getComponent<rtype::components::Mode>(entity);
        const auto size = componentManager.getComponent<components::Size>(entity);
        const auto title = componentManager.getComponent<components::String>(entity);
        const auto created = componentManager.getComponent<components::Created>(entity);
        const auto frameLimit = componentManager.getComponent<components::FrameLimit>(entity);
        if (renderWindow && mode && size && title && frameLimit && !created->isCreate) {
            if (!renderWindow->window)
                renderWindow->window = new sf::RenderWindow();
            renderWindow->window->create(sf::VideoMode(size->width, size->height), title->s, mode->style.style);
            renderWindow->window->setFramerateLimit(frameLimit->limit);
            created->isCreate = true;
            componentManager.addComponent<components::Created>(entity, *created);
        }
    }
}
