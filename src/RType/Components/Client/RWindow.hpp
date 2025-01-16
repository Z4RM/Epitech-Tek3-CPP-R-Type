/*
** EPITECH PROJECT, 2025
** RType
** File description:
** RWindow.hpp
*/


#pragma once
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

namespace rtype::components {
    /**
     * @struct RWindow
     * @brief Manages a render window instance for the RType game.
     *
     * The `RWindow` component encapsulates an SFML `sf::RenderWindow` instance, allowing entities
     * to handle window creation and rendering functionality. It also tracks the window's creation state.
     */
    struct RWindow : public IComponent
    {
        /**
         * @brief Initializes the render window component.
         *
         * Allocates a new `sf::RenderWindow` instance and sets the creation state to false.
         * This method does not open the window; additional configuration and setup
         * are required after initialization.
         *
         * @param value A JSON object containing potential configuration data for the window.
         * This implementation currently does not utilize `value`, but it can be extended
         * for future use to support dynamic window configurations.
         */
        void create(nlohmann::basic_json<> &value) {
            window = new sf::RenderWindow();
            created.isCreate = false;
        }

        /**
         * @brief Pointer to the SFML `sf::RenderWindow` instance.
         *
         * The `window` is dynamically allocated and provides the functionality for rendering
         * graphics, handling input events, and managing the game window. Memory management
         * for this pointer is the responsibility of the component.
         */
        mutable sf::RenderWindow *window;

        /**
         * @struct Created
         * @brief Tracks the creation state of the render window.
         *
         * The `created` field determines whether the window has been successfully created
         * and initialized within the game context.
         */
        Created created;
    };
}
