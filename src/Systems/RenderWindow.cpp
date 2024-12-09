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

void rtype::systems::RenderWindowSys::render(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager)
{
    if (IS_SERVER)
        return;
    for (const auto& entity : entityManager.getEntities()) {
        auto renderWindow = componentManager.getComponent<RWindow>(entity);
        if (!renderWindow)
            continue;
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
        const auto renderWindow = componentManager.getComponent<RWindow>(entity);
        const auto mode = componentManager.getComponent<Mode>(entity);
        const auto size = componentManager.getComponent<Size>(entity);
        const auto title = componentManager.getComponent<String>(entity);
        const auto frameLimit = componentManager.getComponent<FrameLimit>(entity);
        const auto created = componentManager.getComponent<Created>(entity);
        if (renderWindow && mode && size && title && frameLimit && !created->isCreate) {
            const int width = static_cast<int>(size->width);
            const int height = static_cast<int>(size->height);
            renderWindow->window->create(sf::VideoMode(width, height), title->s, mode->style.style);
            renderWindow->window->setFramerateLimit(frameLimit->limit);
            created->isCreate = true;
            componentManager.addComponent<Created>(entity, *created);
        }
    }
}
