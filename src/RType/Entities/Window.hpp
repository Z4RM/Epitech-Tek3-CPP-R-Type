/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Window.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "ECS.hpp"
#include "Components.hpp"

namespace rtype::entities
{
    /**
     * @struct RWindow
     * @brief Wrapper for the `sf::RenderWindow` to enable mutable access.
     *
     * The `RWindow` structure provides a mutable pointer to an `sf::RenderWindow`,
     * allowing flexibility in managing the rendering window.
     */
    struct RWindow
    {
        mutable sf::RenderWindow *window;
    };

    /**
     * @struct Style
     * @brief Encapsulates window style options.
     *
     * The `Style` structure defines various window style flags, such as whether the
     * window has a title bar, is resizable, or starts in full-screen mode.
     */
    struct Style
    {
        bool none = false;
        bool titleBar = false;
        bool resize = false;
        bool close = false;
        bool fullScreen = false;
        bool defaults = true;
        uint32_t style = 0;
    };

    /**
     * @struct Mode
     * @brief Represents the video mode and style configuration for a window.
     *
     * The `Mode` structure combines the video mode, style, and other configurations
     * such as frame rate limit for a window.
     */
    struct Mode
    {
        sf::VideoMode mode;
        Style style;
        unsigned int limit = 60;
    };

    /**
     * @class Window
     * @brief Manages window creation, rendering, and properties.
     *
     * The `Window` class encapsulates the functionality for managing a rendering window,
     * including its properties, configurations, and background sprite.
     */
    class Window {
    public:
        /**
         * @brief Constructs a `Window` instance with the specified parameters.
         *
         * Initializes the `Window` with its associated entity and component managers,
         * window size, title, rendering window, mode, and background sprite.
         *
         * @param entityManager A reference to the `EntityManager` managing entities.
         * @param componentManager A reference to the `ComponentManager` managing components.
         * @param size The size of the window.
         * @param title The title of the window.
         * @param renderWindow A reference to the `RWindow` wrapper for the rendering window.
         * @param mode The `Mode` structure specifying video mode and style.
         * @param backgroundSprite The background sprite for the window.
         */
        Window(
            rtype::ecs::EntityManager &entityManager,
            rtype::ecs::ComponentManager& componentManager,
            components::Size size,
            const components::String &title,
            const RWindow& renderWindow,
            Mode mode
            );

        /**
         * @brief Default destructor for the `Window` class.
         */
        ~Window() = default;

        /**
         * @brief Retrieves the unique ID of the window.
         *
         * Each `Window` instance is assigned a unique ID during creation.
         *
         * @return The unique ID of the window.
         */
        [[nodiscard]] size_t getId() const
        {
            return _id;
        }

    private:
        /**
         * @brief The unique identifier for the window entity.
         */
        size_t _id;
    };
}
