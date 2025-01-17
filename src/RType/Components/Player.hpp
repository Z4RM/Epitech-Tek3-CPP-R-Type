/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#pragma once

#include "Structures.hpp"
#include "ECS.hpp"
#ifdef RTYPE_IS_CLIENT
#include "InputHandler.hpp"
#endif

/**
 * @namespace rtype::components
 * @brief Namespace containing all components used in the R_Type game.
 */
namespace rtype::components {

    /**
     * @class Player
     * @brief Represents a player in the game with various components like position, velocity, size, sprite, and animation.
     *
     * The Player class encapsulates a player entity and manages its properties in the ECS architecture.
     */
    class Player {
    public:
#ifdef RTYPE_IS_CLIENT
        /**
         * @brief Constructs a new Player object (Client version).
         *
         * Initializes a player entity with position, velocity, size, sprite, and animation components.
         *
         * @param entityManager The entity manager handling game entities.
         * @param componentManager The component manager handling game components.
         * @param pos Initial position of the player.
         * @param vel Initial velocity of the player.
         * @param size Size of the player.
         * @param sprite The sprite used for rendering the player.
         * @param animation Animation details of the player.
         */
        Player(
            rtype::ecs::EntityManager &entityManager,
            rtype::ecs::ComponentManager &componentManager,
            Position pos,
            Velocity vel,
            Size size,
            Sprite &sprite,
            const Animation &animation
        );
#else
        /**
         * @brief Constructs a new Player object (Server version).
         *
         * Initializes a player entity with position, velocity, and size components.
         *
         * @param entityManager The entity manager handling game entities.
         * @param componentManager The component manager handling game components.
         * @param pos Initial position of the player.
         * @param vel Initial velocity of the player.
         * @param size Size of the player.
         */
        Player(
            rtype::ecs::EntityManager &entityManager,
            rtype::ecs::ComponentManager &componentManager,
            Position pos,
            Velocity vel,
            Size size
        );
#endif

        /**
         * @brief Destructor for the Player class.
         *
         * Cleans up resources associated with the player object.
         */
        ~Player() = default;

        /**
         * @brief Gets the unique identifier of the player entity.
         *
         * @return The unique identifier of the player entity.
         */
        [[nodiscard]] size_t getId() const;

        /**
         * @brief Moves the player to a new position.
         *
         * @param pos The new position for the player.
         */
        void move(Position pos);

        /**
         * @brief Allows the player to shoot a projectile.
         *
         * @param entityManager The entity manager for creating the projectile entity.
         * @param componentManager The component manager for managing projectile components.
         */
        void shoot(rtype::ecs::EntityManager &entityManager, rtype::ecs::ComponentManager &componentManager) const;

    private:
        /**
         * @brief Unique identifier for the player entity.
         */
        size_t _id;

        /**
         * @brief Clock for managing shooting cooldowns.
         */
        mutable sf::Clock _shootClock;

        /**
         * @brief The cooldown duration for shooting, in seconds.
         */
        const float _shootCooldown = 0.8f;

        /**
         * @brief Flag to determine if the player can shoot projectiles.
         */
        bool _projectile = true;

#ifdef RTYPE_IS_CLIENT
        /**
         * @brief Handles player input on the client side.
         */
        InputHandler _inputs;
#endif
    };
}