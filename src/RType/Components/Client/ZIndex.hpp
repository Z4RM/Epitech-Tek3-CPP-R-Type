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
     * @struct ZIndex
     * @brief Represents the rendering priority of an entity.
     *
     * The `ZIndex` component determines the order in which entities are rendered.
     * Entities with higher `ZIndex` values are rendered on top of those with lower values.
     */
    struct ZIndex : IComponent {
        /**
         * @brief Initializes the ZIndex with a value from a JSON object.
         *
         * This method extracts the `ZIndex` value from the provided JSON and assigns it
         * to the component.
         *
         * @param value A JSON object containing the `ZIndex` value as an integer.
         */
        void create(nlohmann::basic_json<> &value) override {
            this->value = value;
        }

        /**
         * @brief The rendering priority value.
         *
         * Higher values indicate that the entity will be rendered on top of entities
         * with lower `ZIndex` values.
         */
        int value;
    };
}