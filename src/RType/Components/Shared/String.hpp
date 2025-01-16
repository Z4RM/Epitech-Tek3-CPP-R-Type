/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include <string>
#include "IComponent.hpp"

namespace rtype::components {
    /**
     * @struct String
     * @brief Represents a textual string component for an entity.
     *
     * The `String` component stores a single string value, which can be used
     * to represent textual data associated with an entity, such as names,
     * descriptions, or other identifiers.
     */
    struct String : public IComponent {
        /**
         * @brief Initializes the `String` component with a value from a JSON object.
         *
         * This method extracts the string value from the provided JSON object
         * and assigns it to the `s` attribute.
         *
         * @param value A JSON object containing a string value.
         */
        void create(nlohmann::basic_json<>& value) override {
            s = value;
        }

        /**
         * @brief The string value associated with the entity.
         *
         * This attribute stores the textual data for the entity, allowing it
         * to be used in various contexts such as labels, descriptions, or
         * identifiers.
         */
        std::string s;
    };
}
