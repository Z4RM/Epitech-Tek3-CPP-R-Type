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
    };
}

#endif /* !RTYPE_SOUND_HPP_ */
