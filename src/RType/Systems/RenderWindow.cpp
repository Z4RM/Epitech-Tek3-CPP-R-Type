/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RenderWindow.cpp
*/

#include <spdlog/spdlog.h>
#include "InputSystem.hpp"
#include "Components.hpp"
#include "RType/Entities/Window.hpp"
#include "RenderWindow.hpp"

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
        auto renderWindow = componentManager.getComponent<rtype::entities::RWindow>(entity);
        if (!renderWindow)
            continue;

        sf::Event event{};

        while (renderWindow->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderWindow->window->close();
                for (const auto &entity : entityManager.getEntities()) {
                    auto component = componentManager.getComponent<components::Running>(entity);
                    if (component) {
                        component->running = false;
                        break;
                    }
                }
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
        const auto renderWindow = componentManager.getComponent<rtype::entities::RWindow>(entity);
        const auto mode = componentManager.getComponent<rtype::entities::Mode>(entity);
        const auto size = componentManager.getComponent<components::Size>(entity);
        const auto title = componentManager.getComponent<components::String>(entity);
        const auto created = componentManager.getComponent<components::Created>(entity);
        if (renderWindow && mode && size && title && !created->isCreate) {
            renderWindow->window->create(mode->mode, title->s, mode->style.style);
            renderWindow->window->setFramerateLimit(mode->limit);
            created->isCreate = true;
            componentManager.addComponent<components::Created>(entity, *created);
        }
    }
}
