/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Structure.hpp
*/

#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#pragma once
#include <string>
#include <unordered_map>
#ifdef RTYPE_IS_CLIENT
#include <SFML/Graphics.hpp>
#endif

#include <any>

#include "ECS.hpp"
#include "IComponent.hpp"
#include "RType/Components/Shared.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/Components/Client.hpp"
#include "RType/Components/Client/Mode.hpp"
#include "RType/Components/Client/FrameLimit.hpp"
#include "RType/Components/Client/RWindow.hpp"
#include "RType/Components/Client/Speed.hpp"
#include "RType/Components/Client/Music.hpp"
#endif

/**
 * @brief Structure used to dynamically create and register components in the ECS engine.
 */
struct ComponentFactory {
    /**
     * @brief Function to create a unique instance of a component.
     * For example: for a "position" component, this function creates a rtype::components::Position.
     */
    std::function<std::unique_ptr<rtype::components::IComponent>()> creator;
    /**
     * @brief Exact type of the component for identification within the ECS engine.
     * This ensures proper type management and avoids type-related errors.
     */
    std::type_index type;
    /**
     * @brief Function to register the component within the EntityManager and ComponentManager.
     * This function can also initialize related components if required.
     * For example: when registering a "position" component, it can also add a default "velocity" component to the entity.
     */
    std::function<void(rtype::ecs::EntityManager&, rtype::ecs::ComponentManager&, size_t, std::unique_ptr<rtype::components::IComponent>)> registerComponent;
};

/**
 * @brief Map that associates component names with their respective factories.
 * This map is used to dynamically manage components in the ECS engine.
 *
 * Each entry consists of:
 * - A key: The name of the component (e.g., "position", "sprite"). (c.f. Wiki about the available key)
 * - A value: A ComponentFactory containing:
 *   - A function to create the component (creator).
 *   - The exact type of the component (type).
 *   - A function to register the component in the engine (registerComponent).
 */
const std::unordered_map<std::string, ComponentFactory> componentMap = {
    {"position", {
        []() { return std::make_unique<rtype::components::Position>(); },
        typeid(rtype::components::Position),
        [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto position = dynamic_cast<rtype::components::Position*>(instance.get());
            if (position) {
                componentManager.addComponent<rtype::components::Position>(entityId, *position);
                componentManager.addComponent<rtype::components::Velocity>(entityId, rtype::components::Velocity());
            }
        }
    }},
    {"title", {
        []() { return std::make_unique<rtype::components::String>(); },
        typeid(rtype::components::String),
        [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto title = dynamic_cast<rtype::components::String*>(instance.get());
            if (title) {
                componentManager.addComponent<rtype::components::String>(entityId, *title);
            }
        }
    }},
    {"hitbox", {
        []() { return std::make_unique<rtype::components::Size>(); },
        typeid(rtype::components::Size),
        [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto size = dynamic_cast<rtype::components::Size*>(instance.get());
            if (size) {
                componentManager.addComponent<rtype::components::Size>(entityId, *size);
            }
        }
    }},
    {"resolution", {
        []() { return std::make_unique<rtype::components::Size>(); },
        typeid(rtype::components::Size),
        [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto size = dynamic_cast<rtype::components::Size*>(instance.get());
            if (size) {
                componentManager.addComponent<rtype::components::Size>(entityId, *size);
            }
        }
    }},
    {"scrolling_speed", {
        []() { return std::make_unique<rtype::components::Velocity>(); },
        typeid(rtype::components::Velocity),
        [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto velocity = dynamic_cast<rtype::components::Velocity*>(instance.get());
            if (velocity) {
                componentManager.addComponent<rtype::components::Velocity>(entityId, *velocity);
            }
        }
    }},
    {"waves", {
        []() { return std::make_unique<rtype::components::Waves>(); },
        typeid(rtype::components::Waves),
        [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto waves = dynamic_cast<rtype::components::Waves*>(instance.get());
            if (waves) {
                for (auto& enemy : waves->enemies.enemies) {
                    size_t newEntity = entityManager.createEntity();
                    componentManager.addComponent<rtype::components::Enemy>(newEntity, enemy);
                    componentManager.addComponent<rtype::components::Position>(newEntity, enemy.spawn_position);
                    componentManager.addComponent<rtype::components::Velocity>(newEntity, enemy.velocity);
                    componentManager.addComponent<rtype::components::Size>(newEntity, rtype::components::Size());
                    componentManager.addComponent<rtype::components::Projectile>(newEntity, enemy.projectile);
                    componentManager.addComponent<rtype::components::Movement>(newEntity, enemy.movement_pattern);
                    componentManager.addComponent<rtype::components::Created>(newEntity, rtype::components::Created());
#ifdef RTYPE_IS_CLIENT
                    componentManager.addComponent<rtype::components::Sprite>(newEntity, enemy.sprite);
#endif
                }
                componentManager.addComponent<rtype::components::Waves>(entityId, *waves);
            }
        }
    }},
#ifdef RTYPE_IS_CLIENT
{"speed", {
    []() { return std::make_unique<rtype::components::Speed>(); },
    typeid(rtype::components::Speed),
    [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto speed = dynamic_cast<rtype::components::Speed*>(instance.get());
        if (speed) {
            componentManager.addComponent<rtype::components::Speed>(entityId, *speed);
        }
    }
}},
{"action", {
    []() { return std::make_unique<rtype::components::InputHandler>(); },
    typeid(rtype::components::InputHandler),
    [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto action = dynamic_cast<rtype::components::InputHandler*>(instance.get());
        if (action) {
            componentManager.addComponent<rtype::components::InputHandler>(entityId, *action);
        }
    }
}},
{"sprite", {
    []() { return std::make_unique<rtype::components::Sprite>(); },
    typeid(rtype::components::Sprite),
    [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto sprite = dynamic_cast<rtype::components::Sprite*>(instance.get());
        if (sprite) {
            componentManager.addComponent<rtype::components::Sprite>(entityId, *sprite);
            componentManager.addComponent<rtype::components::Created>(entityId, sprite->created);
        }
    }
}},
{"fps_cap", {
    []() { return std::make_unique<rtype::components::FrameLimit>(); },
    typeid(rtype::components::FrameLimit),
    [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto fpsCap = dynamic_cast<rtype::components::FrameLimit*>(instance.get());
        if (fpsCap) {
            componentManager.addComponent<rtype::components::FrameLimit>(entityId, *fpsCap);
        }
    }
}},
{"mode", {
    []() { return std::make_unique<rtype::components::Mode>(); },
    typeid(rtype::components::Mode),
    [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto mode = dynamic_cast<rtype::components::Mode*>(instance.get());
        if (mode) {
            componentManager.addComponent<rtype::components::Mode>(entityId, *mode);
            componentManager.addComponent<rtype::components::RWindow>(entityId, rtype::components::RWindow());
            componentManager.addComponent<rtype::components::Created>(entityId, rtype::components::Created());
        }
    },
}},
{"music", {
    []() { return std::make_unique<rtype::components::Music>(); },
    typeid(rtype::components::Music),
    [](rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto music = dynamic_cast<rtype::components::Music*>(instance.get());
        if (music) {
            componentManager.addComponent<rtype::components::Music>(entityId, *music);
        }
    }
}}
#endif
};

#endif //STRUCTURE_HPP
