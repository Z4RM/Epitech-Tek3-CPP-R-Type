/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RenderWindow.cpp
*/

#include "InputSystem.hpp"
#include "Components.hpp"
#include "RType/Entities/Window.hpp"
#include "RenderWindow.hpp"

#include "RType/Components/Client/SlidingBg.hpp"
#include "RType/Components/Shared/Counter.hpp"

void rtype::systems::RenderWindowSys::render(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager)
{
    static auto lastUpdateTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

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
                        componentManager.addComponent<components::Running>(entity, *component);
                        break;
                    }
                }
                return;
            }
            rtype::systems::InputSystem::handleInput(entityManager, componentManager, event, renderWindow.get());
        }

        auto sortedEntities = _getEntitiesSortedByZIndex(entityManager, componentManager);

        for (auto e : sortedEntities) {
            auto sprite = componentManager.getComponent<components::Sprite>(e.id);
            auto health = componentManager.getComponent<components::Health>(e.id);
            auto dead = componentManager.getComponent<components::Dead>(e.id);

            if (health && health->created && !dead) {
                renderWindow->window->draw(health->bgBar);
                renderWindow->window->draw(health->healthBar);
            }

            //TODO: dont setPos if its already set maybe
            if (sprite && sprite->sprite) {
                auto pos = componentManager.getComponent<components::Position>(e.id);
                auto slide = componentManager.getComponent<components::SlidingBg>(e.id);

                if (slide) {
                    auto position = sprite->sprite->getPosition();
                    position.x -= 150 * elapsedTime.count();

                    //TODO: make it with sprite size
                    if (position.x <= -800) {
                        position.x = 800;
                    }
                    sprite->sprite->setPosition(position);
                }

                if (pos) {
                    sprite->sprite->setPosition({pos->x, pos->y});
                }
                if (!dead)
                    renderWindow->window->draw(*sprite->sprite);
            }
        }

        for (auto e: entityManager.getEntities()) {
            auto text = componentManager.getComponent<components::SfText>(e);
            auto counter = componentManager.getComponent<components::Counter>(e);
            if (text) {
                renderWindow->window->draw(text->text);
            }
            if (counter && counter->text.has_value()) {
                renderWindow->window->draw(counter->text->text);
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

std::vector<rtype::ecs::Entity> rtype::systems::RenderWindowSys::_getEntitiesSortedByZIndex(
        const rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager
) {
    const auto entities = entityManager.getEntities();
    std::vector<rtype::ecs::Entity> renderableEntities;

    for (auto entity: entities) {
        if (componentManager.getComponent<rtype::components::Sprite>(entity)) {
            renderableEntities.push_back({entity});
        }
    }

    std::sort(renderableEntities.begin(), renderableEntities.end(),
              [&componentManager](const rtype::ecs::Entity a,
                                  const rtype::ecs::Entity b) {
                  auto spriteA = componentManager.getComponent<rtype::components::Sprite>(
                          a.id);
                  auto spriteB = componentManager.getComponent<rtype::components::Sprite>(
                          b.id);

                  if (spriteA && spriteB) {
                      return spriteA->priority.value < spriteB->priority.value;
                  }
                  return spriteA != nullptr;
              });

    return renderableEntities;
}
