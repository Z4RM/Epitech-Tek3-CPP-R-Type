/*
** EPITECH PROJECT, 2025
** RType
** File description:
** AnimationProjectile
*/

#ifndef ANIMATIONPROJECTILE_HPP
#define ANIMATIONPROJECTILE_HPP

#include "ECS.hpp"
#include "Structures.hpp"

/**
 * @namespace rtype::systems
 * @brief Contains all system classes used in the R_Type game.
 */
namespace rtype::systems {

    /**
     * @class UpdateProjectilesSystem
     * @brief System for updating projectiles in the ECS architecture.
     *
     * The UpdateProjectilesSystem class provides static functionality to update all projectiles
     * managed by the ECS, ensuring their behavior and animations are properly handled.
     */
    class UpdateProjectilesSystem {
    public:
        /**
         * @brief Updates all projectiles in the game.
         *
         * This function processes all projectile entities, updating their position, animation,
         * or state based on the game logic.
         *
         * @param entityManager The entity manager responsible for handling entities.
         * @param componentManager The component manager responsible for managing components.
         */
        static void updateProjectiles(
            ecs::EntityManager &entityManager,
            ecs::ComponentManager &componentManager
        );
    };

}

#endif // ANIMATIONPROJECTILE_HPP
