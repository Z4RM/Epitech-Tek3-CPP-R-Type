/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Enemy.hpp
*/


#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Components.hpp"
#include "ECS.hpp"

namespace rtype::entites {
    /**
     * @class Enemy
     * @brief Represents an enemy entity in the ECS framework.
     *
     * The `Enemy` class encapsulates the properties and behaviors of enemy entities.
     * It includes support for both client and server configurations, with additional
     * rendering and animation capabilities on the client side.
     */
    class Enemy {
    public:
#ifdef RTYPE_IS_CLIENT
        /**
         * @brief Constructs an enemy entity for the client-side implementation.
         *
         * This constructor initializes an enemy entity with its position, velocity, size,
         * sprite, and animation components. It is used exclusively on the client side,
         * where rendering and animations are required.
         *
         * @param entityManager A reference to the `EntityManager` for managing entities.
         * @param componentManager A reference to the `ComponentManager` for managing components.
         * @param pos The initial position of the enemy.
         * @param vel The velocity of the enemy.
         * @param size The size of the enemy.
         * @param sprite The sprite representing the enemy.
         * @param animation The animation settings for the enemy.
         */
        Enemy(
              rtype::ecs::EntityManager &entityManager,
              rtype::ecs::ComponentManager &componentManager,
              components::Position pos,
              components::Velocity vel,
              components::Size size,
              components::Sprite &sprite,
              const components::Animation &animation
        );
#else
        /**
         * @brief Constructs an enemy entity for the server-side implementation.
         *
         * This constructor initializes an enemy entity with its position, velocity,
         * and size components. It is used exclusively on the server side, where
         * rendering and animations are not required.
         *
         * @param entityManager A reference to the `EntityManager` for managing entities.
         * @param componentManager A reference to the `ComponentManager` for managing components.
         * @param pos The initial position of the enemy.
         * @param vel The velocity of the enemy.
         * @param size The size of the enemy.
         */
        Enemy(
              rtype::ecs::EntityManager &entityManager,
              rtype::ecs::ComponentManager &componentManager,
              components::Position pos,
              components::Velocity vel,
              components::Size size
        );
#endif

        /**
         * @brief Default destructor for the `Enemy` class.
         */
        ~Enemy() = default;

    private:
        /**
         * @brief The unique identifier for the player entity.
         */
        size_t _id;
    };
}


#endif //ENEMY_HPP
