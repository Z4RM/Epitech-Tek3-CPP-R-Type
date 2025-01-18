/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RenderWindow.cpp
*/

#include "RenderWindow.hpp"

#include <iostream>
#include <spdlog/spdlog.h>

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
                  auto spriteA = componentManager.getComponent<rtype::components::Sprite>(a.id);
                  auto spriteB = componentManager.getComponent<rtype::components::Sprite>(b.id);

                  if (spriteA && spriteB) {
                      return spriteA->priority.value < spriteB->priority.value;
                  }
                  return spriteA != nullptr;
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
                return;
            }
            rtype::systems::InputSystem::handleInput(entityManager, componentManager, event, renderWindow);
        }
        auto sortedEntities = getEntitiesSortedByZIndex(entityManager, componentManager);

        for (auto e : sortedEntities) {
            auto sprite = componentManager.getComponent<components::Sprite>(e.id);
            auto health = componentManager.getComponent<components::Health>(e.id);

            if (health && health->created) {
                renderWindow->window->draw(health->bgBar);
                renderWindow->window->draw(health->healthBar);
            }

            //TODO: dont setPos if its already set maybe
            if (sprite && sprite->sprite) {
                auto pos = componentManager.getComponent<components::Position>(e.id);
                if (pos)
                    sprite->sprite->setPosition({pos->x, pos->y});
                renderWindow->window->draw(*sprite->sprite);
            }
        }

        for (auto e: entityManager.getEntities()) {
            auto text = componentManager.getComponent<components::SfText>(e);
            if (text) {
                renderWindow->window->draw(text->text);
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
