/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Movement.hpp
*/

#pragma once
#include "ECS.hpp"
#include "RType/Components/Shared/Position.hpp"
#include "RType/Components/Shared/Hitbox.hpp"
#include "RType/Components/Shared/Velocity.hpp"

namespace rtype::systems
{
    /**
     * @class Movement
     * @brief Static class for handling entity movement in the ECS system.
     *
     * The `Movement` class provides a static method to process and update
     * the movement of entities based on their components. This class cannot
     * be instantiated or destroyed as it is purely static.
     */
    class Movement {
    public:
        /**
         * @brief Deleted constructor to prevent instantiation.
         */
        Movement() = delete;

        /**
         * @brief Deleted destructor to prevent instantiation.
         */
        ~Movement() = delete;

        /**
         * @brief Updates the movement of entities in the ECS system.
         *
         * Iterates through the entities in the `EntityManager` and updates
         * their positions based on the movement-related components in the
         * `ComponentManager`.
         *
         * @param entityManager A reference to the `EntityManager` containing
         *        all entities in the ECS system.
         * @param componentManager A reference to the `ComponentManager` that
         *        manages all components in the ECS system.
         */
        static void move(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);

        /**
         * @brief Initiates movement for entities by updating their velocity.
         * This function iterates through all entities in the game, checks their
         * associated movement components, and updates their velocity based on
         * the movement type and associated parameters.
         *
         * @param entityManager The entity manager responsible for managing entities.
         * @param componentManager The component manager responsible for managing components.
         */
        static void startMove(const ecs::EntityManager& entityManager, ecs::ComponentManager& componentManager);

        /**
         * @brief Handle all the possible collisions with other entities
         * @param entity The current entity
         * @param pos The current entity's position
         * @param hitBox The current entity's hitBox
         * @param entities All present entities given by the entityManager
         * @param componentManager A reference to the `ComponentManager` that
         *        manages all components in the ECS system.
         * @param vel The current entity's velocity
         */
        static void
        handleCollisions(unsigned int entity, components::Position *pos, components::Hitbox *hitBox,
                         const std::unordered_set<unsigned int> &entities, ecs::ComponentManager &componentManager,
                         components::Velocity *vel);

        /**
         * @brief Check if an entity is colliding with another one
         * @param pos The "A" entity's position
         * @param hitBox The "A" entity's hitBox
         * @param colliderPos The "B" entity's position
         * @param colliderHitBox The "B" entity's hitBox
         * @return True if a collision occurs
         */
        static bool
        isColliding(components::Position *pos, components::Hitbox *hitBox, components::Position *colliderPos,
                    components::Hitbox *colliderHitBox);

        /**
         * @brief Calculate the distance between two positions
         * @param pos1 The first position
         * @param pos2 The second position
         * @return The distance between the two coordinates
         */
        static float getDistanceBetweenPositions(const components::Position *pos1, const components::Position *pos2);
    };
}
