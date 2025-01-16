/*
** EPITECH PROJECT, 2025
** RType
** File description:
** FrameLimit.hpp
*/


#pragma once
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

namespace rtype::components {
    /**
     * @brief Component that defines the frame rate limit for an entity.
     *
     * The `FrameLimit` component stores a frame rate limit value that can be used
     * to control the speed or timing behavior of a game entity.
     */
    struct FrameLimit : public IComponent
    {
        /**
         * @brief Initializes the frame limit component with JSON data.
         *
         * This method extracts the frame limit value from the provided JSON object
         * and stores it in the `limit` attribute.
         *
         * @param value A reference to a JSON object containing the frame limit value.
         */
        void create(nlohmann::basic_json<> &value) override {
            limit = value;
        }

        /**
         * @brief The maximum number of frames per second (FPS) for the associated entity.
         */
        int limit;
    };
}
