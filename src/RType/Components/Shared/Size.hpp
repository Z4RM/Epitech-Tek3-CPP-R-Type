/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "IComponent.hpp"

namespace rtype::components {
    /**
     * @struct Size
     * @brief Represents the dimensions of an entity in the game world.
     *
     * The `Size` component stores the width and height of an entity, typically
     * used for defining its visual or physical boundaries.
     */
    struct Size : public IComponent {
        /**
         * @brief Initializes the `Size` component with values from a JSON object.
         *
         * This method reads the `width` and `height` fields from the provided JSON
         * object and assigns them to the corresponding attributes.
         *
         * @param value A JSON object containing the `width` and `height` fields.
         */
        void create(nlohmann::basic_json<> &value) override {
            width = value["width"];
            height = value["height"];
        }

        /**
         * @brief The width of the entity.
         *
         * Represents the horizontal size of the entity in the game world.
         */
        float width;

        /**
         * @brief The height of the entity.
         *
         * Represents the vertical size of the entity in the game world.
         */
        float height;
    };
}