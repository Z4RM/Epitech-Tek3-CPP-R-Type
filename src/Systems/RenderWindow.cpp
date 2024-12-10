/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RenderWindow.cpp
*/

#include "RenderWindow.hpp"

#include <iostream>

#include "RType/ModeManager/ModeManager.hpp"

std::vector<rtype::ecs::Entity> getEntitiesSortedByZIndex(
    const rtype::ecs::EntityManager& entityManager,
    rtype::ecs::ComponentManager& componentManager
) {
    const auto entities = entityManager.getEntities();
    std::vector<rtype::ecs::Entity> renderableEntities;

    for (auto entity : entities) {
        if (componentManager.getComponent<Sprite>(entity)) {
            renderableEntities.push_back({entity});
        }
    }

    std::sort(renderableEntities.begin(), renderableEntities.end(),
              [&componentManager](const rtype::ecs::Entity a, const rtype::ecs::Entity b) {
                  auto zIndexA = componentManager.getComponent<Sprite>(a.id)->priority.value;
                  auto zIndexB = componentManager.getComponent<Sprite>(b.id)->priority.value;
                  return zIndexA < zIndexB;
              });

    return renderableEntities;
}

void rtype::systems::RenderWindowSys::render(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager)
{
    if (IS_SERVER)
        return;
    for (const auto& entity : entityManager.getEntities()) {
        auto renderWindow = componentManager.getComponent<rtype::components::RWindow>(entity);
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
            auto sprite = componentManager.getComponent<Sprite>(e.id);
            if (sprite && sprite->sprite) {
                renderWindow->window->draw(*sprite->sprite);
            }
        }
        renderWindow->window->display();
    }
}

void rtype::systems::RenderWindowSys::createWindow(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager)
{
    if (IS_SERVER)
        return;
    for (const auto& entity : entityManager.getEntities()) {
        const auto renderWindow = componentManager.getComponent<rtype::components::RWindow>(entity);
        const auto mode = componentManager.getComponent<rtype::components::Mode>(entity);
        const auto size = componentManager.getComponent<Size>(entity);
        const auto title = componentManager.getComponent<String>(entity);
        const auto created = componentManager.getComponent<Created>(entity);
        if (renderWindow && mode && size && title && !created->isCreate) {
            renderWindow->window->create(mode->mode, title->s, mode->style.style);
            renderWindow->window->setFramerateLimit(mode->limit);
            created->isCreate = true;
            componentManager.addComponent<Created>(entity, *created);
        }
    }
}
