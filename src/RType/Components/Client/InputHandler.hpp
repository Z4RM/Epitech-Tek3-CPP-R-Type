/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputHandler.hpp
*/

/**
 * @file InputHandler.hpp
 * @brief Header file for the InputHandler component in the RType project.
 *
 * This file defines the `InputHandler` struct, which maps keyboard keys
 * to specific actions represented as functions.
 */

#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <functional>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "IComponent.hpp"
#include "GameLoader.hpp"
#include "spdlog/spdlog.h"
#include "RType/Components/Shared/Speed.hpp"

namespace rtype::components {
    /**
     * @struct InputHandler
     * @brief Manages keyboard input actions for entities.
     *
     * The `InputHandler` component enables entities to respond to keyboard inputs.
     * It maps keyboard keys to specific actions and associates those actions
     * with event types (e.g., key press or release) and their corresponding
     * behaviors.
     */
    struct InputHandler : public IComponent {
        /**
         * @brief Mapping of string keys to `sf::Keyboard::Key` values.
         *
         * This map allows configuration files to refer to keys by their string names
         * (e.g., "Z", "Space") and converts them to SFML's `sf::Keyboard::Key` values.
         */
        std::unordered_map<std::string, sf::Keyboard::Key> keyMap = {
            {"Z", sf::Keyboard::Key::Z},
            {"S", sf::Keyboard::Key::S},
            {"Q", sf::Keyboard::Key::Q},
            {"D", sf::Keyboard::Key::D},
            {"Space", sf::Keyboard::Key::Space}
        };

        /**
         * @brief Mapping of string event types to `sf::Event::EventType`.
         *
         * This map enables configuration files to specify event types
         * like "Pressed" or "Released", which are then mapped to the corresponding
         * SFML event types.
         */
        std::unordered_map<std::string, sf::Event::EventType> eventMap = {
            {"Pressed", sf::Event::EventType::KeyPressed},
            {"Released", sf::Event::EventType::KeyReleased}
        };

        /**
         * @brief Mapping of string actions to their corresponding functions.
         *
         * This map binds action names (e.g., "goUp", "goDown") to lambda functions
         * that execute the desired behavior for an entity when the action is triggered.
         */
        std::unordered_map<std::string, std::function<void(ecs::ComponentManager& componentManager, size_t id)>> functionMap = {
            {"goUp", [this](ecs::ComponentManager& componentManager, size_t id) {
                auto speed = componentManager.getComponent<Speed>(id);
                if (speed) {
                    componentManager.getComponent<Velocity>(id)->y = -(speed->value);
                } else {
                    componentManager.getComponent<Velocity>(id)->y = -1;
                }
            }},
            {"goDown", [this](ecs::ComponentManager& componentManager, size_t id) {
                auto speed = componentManager.getComponent<Speed>(id);
                if (speed) {
                    componentManager.getComponent<Velocity>(id)->y = speed->value;
                } else {
                    componentManager.getComponent<Velocity>(id)->y = 1;
                }
            }},
            {"goLeft", [this](ecs::ComponentManager& componentManager, size_t id) {
                auto speed = componentManager.getComponent<Speed>(id);
                if (speed) {
                    componentManager.getComponent<Velocity>(id)->x = -(speed->value);
                } else {
                    componentManager.getComponent<Velocity>(id)->x = -1;
                }
            }},
            {"goRight", [this](ecs::ComponentManager& componentManager, size_t id) {
                auto speed = componentManager.getComponent<Speed>(id);
                if (speed) {
                    componentManager.getComponent<Velocity>(id)->x = (speed->value);
                } else {
                    componentManager.getComponent<Velocity>(id)->x = 1;
                }
            }},
            {"stopX", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->x = 0;
            }},
            {"stopY", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->y = 0;
            }}
        };

        /**
         * @brief Initializes the `InputHandler` component from a JSON configuration.
         *
         * Reads key mappings, event types, and associated actions from a JSON object,
         * and populates the `keyActions` map with the corresponding behaviors.
         *
         * @param value A JSON object containing input configurations.
         */
        void create(nlohmann::basic_json<> &value) override {
            GameLoader inputLoader;
            inputLoader.loadGlobalConfig(value);

            for (auto& input : inputLoader.globalConfig.items()) {
                try {
                    auto key = keyMap.at(input.value()["key"]);
                    auto event = eventMap.at(input.value()["type"]);
                    auto function = functionMap.at(input.value()["action"]);

                    keyActions.insert({key, {event, function}});
                } catch (const std::out_of_range &e) {
                    spdlog::error("Invalid input: {}", input.key());
                }
            }
        }

        /**
         * @brief A multimap associating keyboard keys with actions.
         *
         * Each key in the map corresponds to an `sf::Keyboard::Key`, and each action
         * is represented as a pair of `sf::Event::EventType` and a `std::function`
         * to execute the action.
         */
        std::unordered_multimap<sf::Keyboard::Key, std::pair<sf::Event::EventType, std::function<void(ecs::ComponentManager& componentManager, size_t id)>>> keyActions;
    };
}

#endif //INPUTHANDLER_HPP
