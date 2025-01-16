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
     * @struct Created
     * @brief Indicates whether a component or entity has been initialized.
     *
     * The `Created` component is used to track whether a specific component or entity
     * has been successfully created and initialized within the system.
     */
    struct Created : public IComponent {
        /**
         * @brief Initializes the `Created` component with a value from a JSON object.
         *
         * This method reads the `isCreate` value from the provided JSON object
         * and updates the component's state.
         *
         * @param value A JSON object containing the `isCreate` field.
         */
        void create(nlohmann::basic_json<> &value) override {
            isCreate = value["isCreate"];
        }

        /**
         * @brief A flag indicating the creation status of the component.
         *
         * - `true`: The component or entity has been successfully created.
         * - `false`: The component or entity is not yet created or initialized.
         */
        bool isCreate = false;
    };
}