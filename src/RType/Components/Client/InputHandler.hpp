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

namespace rtype::components {
    /**
     * @struct InputHandler
     * @brief Manages keyboard input actions for entities.
     *
     * The `InputHandler` component allows associating keyboard keys with
     * specific actions using a map. This enables easy handling of input-driven
     * behavior in the RType game.
     */
    struct InputHandler : public IComponent {

        const std::unordered_map<std::string, sf::Keyboard::Key> keyMap = {
            {"Z", sf::Keyboard::Key::Z},
            {"S", sf::Keyboard::Key::S},
            {"Q", sf::Keyboard::Key::Q},
            {"D", sf::Keyboard::Key::D},
            {"Space", sf::Keyboard::Key::Space}
        };

        const std::unordered_map<std::string, sf::Event::EventType> eventMap = {
            {"Pressed", sf::Event::EventType::KeyPressed},
            {"Released", sf::Event::EventType::KeyReleased}
        };

        const std::unordered_map<std::string, std::function<void(ecs::ComponentManager& componentManager, size_t id)>> functionMap = {
            {"goUp", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->y = -1;
            }},
            {"goDown", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->y = 1;
            }},
            {"goLeft", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->x = -1;
            }},
            {"goRight", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->x = 1;
            }},
            {"stopX", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->x = 0;
            }},
            {"stopY", [this](ecs::ComponentManager& componentManager, size_t id) {
                componentManager.getComponent<Velocity>(id)->y = 0;
            }}
        };

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
         * is represented as a pair of `sf::Event::EventType` `std::function<void()>`. This allows flexible handling
         * of key presses, binding specific functions to them.
         */
        std::unordered_multimap<sf::Keyboard::Key, std::pair<sf::Event::EventType, std::function<void(ecs::ComponentManager& componentManager, size_t id)>>> keyActions;
    };
}

#endif //INPUTHANDLER_HPP
