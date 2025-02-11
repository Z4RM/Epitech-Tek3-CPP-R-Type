/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Sound.hpp
*/

#ifndef RTYPE_SOUND_HPP_
#define RTYPE_SOUND_HPP_

#include "ECS.hpp"

namespace rtype::systems {
    /**
     * @class SoundSys
     *
     * @brief Manages sounds in the ECS framework.
     */
    class Sound {
    public:
        /**
         * @brief Plays sounds in the ECS system.
         *
         * This method processes and plays all sounds managed by the `EntityManager`
         * and their associated components managed by the `ComponentManager`.
         *
         * @param entityManager A reference to the `EntityManager` that holds all entities in the ECS system.
         * @param componentManager A reference to the `ComponentManager` that manages drawable and rendering-related components.
         */
        static void play(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);

        /**
         * @brief Creates a new sound effect using the ECS framework.
         *
         * @param path The path to the sound effect file.
         * @param componentManager A reference to the `ComponentManager` for managing sound-related components.
         * @param entity The entity ID to associate with the new sound effect.
         */
        static void createEffect(const std::string &path, ecs::ComponentManager &componentManager, unsigned int entity);

        /**
         * @brief Creates a new music using the ECS framework.
         *
         * @param path The path to the music file.
         * @param componentManager A reference to the `ComponentManager` for managing music-related components.
         * @param entity The entity ID to associate with the new music.
         * @param loop Whether the music should loop or not.
         * @param duration The duration of the music, if you want to play only a part of it (optional).
         */
        static void createMusic(
                const std::string &path,
                ecs::ComponentManager &componentManager,
                unsigned int entity,
                bool loop,
                std::optional<float> duration
        );
    };
}

#endif /* !RTYPE_SOUND_HPP_ */
