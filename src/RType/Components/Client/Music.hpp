/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Music.hpp
*/


#pragma once
#include <SFML/Audio.hpp>
#include "IComponent.hpp"
#include "spdlog/spdlog.h"

namespace rtype::components {
    /**
     * @struct Music
     * @brief Handles background music playback for entities.
     *
     * The `Music` component provides functionality to load, configure, and play
     * background music from a specified file. It utilizes the SFML `sf::Music`
     * class for audio playback.
     */
    struct Music : public IComponent
    {
        /**
         * @brief Initializes the music component with the given configuration.
         *
         * Loads the music file from the specified path and configures playback
         * options such as looping and volume. The music is automatically played
         * once it is successfully loaded.
         *
         * @param value A JSON object containing the following keys:
         * - `"path"`: (string) The file path to the music file.
         * - `"loop"`: (boolean) Whether the music should loop.
         * - `"volume"`: (float) The volume level of the music (0 to 100).
         *
         * @note If the music file cannot be loaded, an error message is printed
         * to the standard error stream, and playback is not started.
         */
        void create(nlohmann::basic_json<> &value) override {
            std::string tmp = "../";
            tmp.append(value["path"]);
            if (!music->openFromFile(tmp)) {
                spdlog::error("Error loading music from {}", tmp);
                return;
            }

            music->setLoop(value["loop"]);
            music->setVolume(value["volume"]);
            music->play();
        }

        /**
         * @brief Pointer to the SFML `sf::Music` object.
         *
         * The `sf::Music` instance manages the audio playback, including loading
         * the audio file, setting playback properties, and controlling playback
         * state. The memory for this object is dynamically allocated.
         */
        sf::Music *music = new sf::Music();
    };
}
