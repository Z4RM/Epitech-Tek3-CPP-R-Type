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
     * @struct Position
     * @brief Represents the 3D position of an entity in the game world.
     *
     * The `Position` component stores the coordinates of an entity in a
     * 3D space, including the X, Y, and Z axes.
     */
    struct Position : public IComponent {
        /**
         * @brief Initializes the `Position` component with values from a JSON object.
         *
         * This method reads the `x`, `y`, and `z` fields from the provided JSON object
         * and assigns them to the corresponding attributes.
         *
         * @param value A JSON object containing the `x`, `y`, and `z` fields.
         */
        void create(nlohmann::basic_json<>& value) override {
            x = value["x"];
            y = value["y"];
            z = value["z"];
        }

        /**
         * @brief The X-coordinate of the entity's position in the game world.
         */
        float x;

        /**
         * @brief The Y-coordinate of the entity's position in the game world.
         */
        float y;

        /**
         * @brief The Z-coordinate of the entity's position in the game world.
         *
         * This is often used for depth or layering purposes in a 2.5D or 3D environment.
         */
        float z;
    };
}
