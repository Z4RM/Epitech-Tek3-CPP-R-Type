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
     * @struct Velocity
     * @brief Represents the velocity of an entity in 3D space.
     *
     * The `Velocity` component defines the movement speed and direction
     * of an entity along the X, Y, and Z axes.
     */
    struct Velocity : public IComponent {
        /**
         * @brief Initializes the `Velocity` component with values from a JSON object.
         *
         * This method extracts the velocity values along the X, Y, and Z axes
         * from the provided JSON object and assigns them to the respective attributes.
         *
         * @param value A JSON object containing the velocity values.
         * The object must include the keys "x", "y", and "z".
         */
        void create(nlohmann::basic_json<> &value) override {
            x = value["x"];
            y = value["y"];
            z = value["z"];
        }

        /**
         * @brief The velocity along the X-axis.
         */
        float x;

        /**
         * @brief The velocity along the Y-axis.
         */
        float y;

        /**
         * @brief The velocity along the Z-axis.
         */
        float z;
    };
}