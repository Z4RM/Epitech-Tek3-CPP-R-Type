/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "../Shared/Position.hpp"
#include "../Shared/Created.hpp"
#include "../Shared/Size.hpp"
#include "../Client/ZIndex.hpp"
#include "IComponent.hpp"

namespace rtype::components {
    /**
     * @struct Sprite
     * @brief Manages the graphical representation of an entity.
     *
     * The `Sprite` component handles loading, scaling, and rendering textures
     * for entities. It supports priority rendering through the `ZIndex` and defines
     * the size and shape of the graphical entity.
     */
    struct Sprite : public IComponent
    {
        /**
         * @brief Initializes the sprite with values from a JSON object.
         *
         * This method configures the sprite by loading its texture, setting its size,
         * and initializing its graphical components.
         *
         * @param value A JSON object containing the sprite configuration.
         * - `path` (string): Path to the texture file.
         * - `priority` (object): A JSON object specifying the rendering priority.
         * - `size` (object): A JSON object with `width` and `height` fields for the sprite size.
         */
        void create(nlohmann::basic_json<> &value) override {
            path = value["path"];
            priority.create(value["priority"]);
            created.isCreate = true;
            texture = new sf::Texture();
            sprite = new sf::Sprite();
            rect = new sf::RectangleShape();
            size = Size();
            size.width = value["size"]["width"];
            size.height = value["size"]["height"];
            texture->loadFromFile(path);
            sprite->setTexture(*texture);
            sprite->setScale(size.width / texture->getSize().x, size.height / texture->getSize().y);
            rect->setSize({size.width, size.height});
        }

        /**
         * @brief Path to the texture file.
         *
         * Specifies the location of the texture to be loaded and used for the sprite.
         */
        std::string path;

        /**
         * @brief Rendering priority for the sprite.
         *
         * Determines the order in which the sprite is rendered relative to others.
         * Higher priorities are rendered last (on top).
         */
        ZIndex priority;

        /**
         * @brief Pointer to the texture of the sprite.
         *
         * The texture is loaded from the file specified in `path` and applied to the sprite.
         */
        sf::Texture* texture;

        /**
         * @brief Pointer to the SFML sprite object.
         *
         * Represents the graphical entity and handles rendering within the game.
         */
        sf::Sprite* sprite;

        /**
         * @brief Pointer to the rectangle shape representing the sprite.
         *
         * Used for defining the size and boundaries of the sprite.
         */
        sf::RectangleShape* rect;

        /**
         * @brief Tracks whether the sprite has been created.
         *
         * Used to ensure the component is initialized before rendering or interaction.
         */
        Created created;

        /**
         * @brief Dimensions of the sprite.
         *
         * Specifies the width and height of the sprite, used for scaling and collision.
         */
        Size size;
    };
}
