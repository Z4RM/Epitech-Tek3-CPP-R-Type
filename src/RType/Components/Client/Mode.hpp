/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Mode.hpp
*/


#pragma once
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

/**
 * @struct Style
 * @brief Represents the configuration of window styles for an entity.
 *
 * The `Style` structure holds flags that determine which styles should be applied
 * to the SFML window. It is used in the `Mode` component to configure window
 * properties such as title bar, resizeability, fullscreen mode, etc.
 */
struct Style {
    /**
     * @brief No window style, equivalent to `sf::Style::None`.
     *
     * When set to `true`, the window will have no specific style applied.
     * Defaults to `false`.
     */
    bool none = false;

    /**
     * @brief Enables the title bar, equivalent to `sf::Style::Titlebar`.
     *
     * When set to `true`, the window will have a title bar. Defaults to `false`.
     */
    bool titleBar = false;

    /**
     * @brief Enables window resizing, equivalent to `sf::Style::Resize`.
     *
     * When set to `true`, the window will be resizable. Defaults to `false`.
     */
    bool resize = false;

    /**
     * @brief Enables the close button, equivalent to `sf::Style::Close`.
     *
     * When set to `true`, the window will have a close button. Defaults to `false`.
     */
    bool close = false;

    /**
     * @brief Enables fullscreen mode, equivalent to `sf::Style::Fullscreen`.
     *
     * When set to `true`, the window will be displayed in fullscreen mode.
     * Defaults to `false`.
     */
    bool fullScreen = false;

    /**
     * @brief Enables the default window style, equivalent to `sf::Style::Default`.
     *
     * When set to `true`, the default window style will be used. Defaults to `true`.
     */
    bool defaults = true;

    /**
     * @brief The final calculated window style bitmask.
     *
     * This field is a combination of the selected styles using bitwise OR.
     * It is calculated based on the values of the other flags in the structure.
     * The bitmask is used to apply the final style to the SFML window.
     */
    uint32_t style = sf::Style::Default;  ///< Default style is applied initially.
};

namespace rtype::components {
    /**
     * @struct Mode
     * @brief Configures the window's style options for an entity.
     *
     * The `Mode` component allows the specification of window styles such as
     * title bar, resizable, fullscreen, etc., based on input JSON configuration.
     * It uses a `Style` structure to store the flags and calculates the final
     * style bitmask to be applied to the SFML window.
     */
    struct Mode : public IComponent
    {
        /**
         * @brief Initializes the `Mode` component from a JSON configuration.
         *
         * Reads the boolean style flags (e.g., "none", "titleBar") from the JSON
         * input and calculates the corresponding SFML style bitmask.
         *
         * @param value A JSON object containing style configuration.
         */
        void create(nlohmann::basic_json<> &value) override {
            style.none = value["none"] || false;
            style.titleBar = value["titleBar"] || false;
            style.resize = value["resize"] || false;
            style.close = value["close"] || false;
            style.fullScreen = value["fullscreen"] || false;
            style.defaults = value["defaults"] || true;
            style.style = style.none * sf::Style::None
                + style.titleBar * sf::Style::Titlebar
                + style.resize * sf::Style::Resize
                + style.close * sf::Style::Close
                + style.fullScreen * sf::Style::Fullscreen
                + style.defaults * sf::Style::Default;

        }
        Style style;  ///< Stores the window style configuration.
    };
}
