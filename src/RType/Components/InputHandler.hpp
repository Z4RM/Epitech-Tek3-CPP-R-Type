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
#include <SFML/Window/Keyboard.hpp>

namespace rtype::components {
    /**
     * @struct InputHandler
     * @brief Manages keyboard input actions for entities.
     *
     * The `InputHandler` component allows associating keyboard keys with
     * specific actions using a map. This enables easy handling of input-driven
     * behavior in the RType game.
     */
    struct InputHandler {
        /**
         * @brief A map associating keyboard keys with actions.
         *
         * Each key in the map corresponds to an `sf::Keyboard::Key`, and each action
         * is represented as a `std::function<void()>`. This allows flexible handling
         * of key presses, binding specific functions to them.
         */
        std::unordered_map<sf::Keyboard::Key, std::function<void()>> keyActions;
    };
}

#endif //INPUTHANDLER_HPP
