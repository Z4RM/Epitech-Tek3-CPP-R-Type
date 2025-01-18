/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Speed.hpp
*/


#pragma once
#include "IComponent.hpp"

namespace rtype::components {
    /**
     * @struct Speed
     * @brief Represents the movement speed of an entity.
     *
     * The `Speed` component defines the speed value for an entity, which can be
     * used in conjunction with other components, such as `Velocity`, to calculate
     * movement behavior within the game.
     */
    struct Speed : public IComponent
    {
        /**
         * @brief Initializes the speed component with a value.
         *
         * This method assigns the speed value from the provided JSON object.
         *
         * @param value A JSON object containing the speed value.
         * The JSON should contain a numerical value directly assignable to the `value` field.
         */
        void create(nlohmann::basic_json<> &value) override {
            this->value = value;
        }

        /**
         * @brief The movement speed of the entity.
         *
         * This integer value represents the speed at which the entity moves.
         * Higher values result in faster movement.
         */
        int value;
    };
}
