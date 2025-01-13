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
 * Declaration of a component in componentsMap:
 * {"[NAME]", []() {return [LINKED_COMPONENT];}},
 */
struct ComponentFactory {
    std::function<std::unique_ptr<rtype::components::IComponent>()> creator;
    std::type_index type;
    std::function<void(rtype::ecs::ComponentManager&, size_t, std::unique_ptr<rtype::components::IComponent>)> registerComponent;
};

// TODO: Add documentation for the list and separate components of the client and server.
const std::unordered_map<std::string, ComponentFactory> componentMap = {
    {"position", {
        []() { return std::make_unique<rtype::components::Position>(); },
        typeid(rtype::components::Position),
        [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto position = dynamic_cast<rtype::components::Position*>(instance.get());
            if (position) {
                manager.addComponent<rtype::components::Position>(entityId, *position);
                manager.addComponent<rtype::components::Velocity>(entityId, rtype::components::Velocity());
            }
        }
    }},
    {"title", {
        []() { return std::make_unique<rtype::components::String>(); },
        typeid(rtype::components::String),
        [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto title = dynamic_cast<rtype::components::String*>(instance.get());
            if (title) {
                manager.addComponent<rtype::components::String>(entityId, *title);
            }
        }
    }},
    {"hitbox", {
        []() { return std::make_unique<rtype::components::Size>(); },
        typeid(rtype::components::Size),
        [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto size = dynamic_cast<rtype::components::Size*>(instance.get());
            if (size) {
                manager.addComponent<rtype::components::Size>(entityId, *size);
            }
        }
    }},
    {"resolution", {
        []() { return std::make_unique<rtype::components::Size>(); },
        typeid(rtype::components::Size),
        [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto size = dynamic_cast<rtype::components::Size*>(instance.get());
            if (size) {
                manager.addComponent<rtype::components::Size>(entityId, *size);
            }
        }
    }},
    {"scrolling_speed", {
        []() { return std::make_unique<rtype::components::Velocity>(); },
        typeid(rtype::components::Velocity),
        [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
            auto velocity = dynamic_cast<rtype::components::Velocity*>(instance.get());
            if (velocity) {
                manager.addComponent<rtype::components::Velocity>(entityId, *velocity);
            }
        }
    }},
#ifdef RTYPE_IS_CLIENT
{"speed", {
    []() { return std::make_unique<rtype::components::Speed>(); },
    typeid(rtype::components::Speed),
    [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto speed = dynamic_cast<rtype::components::Speed*>(instance.get());
        if (speed) {
            manager.addComponent<rtype::components::Speed>(entityId, *speed);
        }
    }
}},
{"action", {
    []() { return std::make_unique<rtype::components::InputHandler>(); },
    typeid(rtype::components::InputHandler),
    [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto action = dynamic_cast<rtype::components::InputHandler*>(instance.get());
        if (action) {
            manager.addComponent<rtype::components::InputHandler>(entityId, *action);
        }
    }
}},
{"sprite", {
    []() { return std::make_unique<rtype::components::Sprite>(); },
    typeid(rtype::components::Sprite),
    [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto sprite = dynamic_cast<rtype::components::Sprite*>(instance.get());
        if (sprite) {
            manager.addComponent<rtype::components::Sprite>(entityId, *sprite);
            manager.addComponent<rtype::components::Created>(entityId, sprite->created);
        }
    }
}},
{"fps_cap", {
    []() { return std::make_unique<rtype::components::FrameLimit>(); },
    typeid(rtype::components::FrameLimit),
    [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto fpsCap = dynamic_cast<rtype::components::FrameLimit*>(instance.get());
        if (fpsCap) {
            manager.addComponent<rtype::components::FrameLimit>(entityId, *fpsCap);
        }
    }
}},
{"mode", {
    []() { return std::make_unique<rtype::components::Mode>(); },
    typeid(rtype::components::Mode),
    [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto mode = dynamic_cast<rtype::components::Mode*>(instance.get());
        if (mode) {
            manager.addComponent<rtype::components::Mode>(entityId, *mode);
            manager.addComponent<rtype::components::RWindow>(entityId, rtype::components::RWindow());
            manager.addComponent<rtype::components::Created>(entityId, rtype::components::Created());
        }
    },
}},
{"music", {
    []() { return std::make_unique<rtype::components::Music>(); },
    typeid(rtype::components::Music),
    [](rtype::ecs::ComponentManager& manager, const size_t entityId, const std::unique_ptr<rtype::components::IComponent> &instance) {
        auto music = dynamic_cast<rtype::components::Music*>(instance.get());
        if (music) {
            manager.addComponent<rtype::components::Music>(entityId, *music);
        }
    }
}}
#endif
};

#endif //STRUCTURE_HPP
