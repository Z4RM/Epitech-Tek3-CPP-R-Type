/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#pragma once

#include "Components.hpp"
#include "ECS.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/Components/Shared/ActualPlayer.hpp"
#ifdef RTYPE_IS_CLIENT
#include "Components.hpp"
#endif

/**
 * @class Player
 * @brief Represents a player in the game with position, velocity, size, sprite, and animation components.
 *
 * This class encapsulates the player entity and its associated components in the ECS architecture.
 */
namespace rtype::entities {
    class Player {
    public:
#ifdef RTYPE_IS_CLIENT
        bool shoot(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, size_t id,
        std::chrono::steady_clock::time_point &clock, bool isSuperProjectile);
        std::chrono::steady_clock::time_point _elaspedShoot= std::chrono::steady_clock::now();
        double _shootCooldown = 0.8;
        /**
         * @brief Constructs a new Player object.
         *
         * Initializes a player entity with its components in the ECS (Entity-Component-System) architecture.
         *
         * @param entityManager The entity manager responsible for handling entities.
         * @param componentManager The component manager responsible for handling components.
         * @param pos The initial position of the player.
         *        - x: The x-coordinate in the game world.
         *        - y: The y-coordinate in the game world.
         *        - z: The z-coordinate (depth) in the game world.
         * @param vel The initial velocity of the player.
         *        - x: Horizontal velocity of the player.
         *        - y: Vertical velocity of the player.
         * @param size The size of the player.
         *        - width: The width of the player.
         *        - height: The height of the player.
         * @param sprite The sprite used to render the player.
         *        - pos: The position of the sprite in the game world.
         *        - size: The dimensions of the sprite.
         *        - path: The file path to the sprite image.
         * @param animation The animation details of the player.
         *        - path: The file path to the animation data.
         *        - nbFrames: The total number of frames in the animation.
         *        - frameRate: The frame rate of the animation.
         */
        Player(
                rtype::ecs::EntityManager &entityManager,
                rtype::ecs::ComponentManager &componentManager,
                components::Position pos,
                components::Velocity vel,
                components::Size size,
                components::Sprite &sprite,
                const components::Animation &animation,
                rtype::components::NetId = { false },
                rtype::components::ActualPlayer = { false },
                rtype::components::Speed = {200}
        );

#endif

        /**
         * @brief Constructs a new Player object.
         *
         * Initializes a player entity with its components in the ECS (Entity-Component-System) architecture.
         *
         * @param entityManager The entity manager responsible for handling entities.
         * @param componentManager The component manager responsible for handling components.
         * @param pos The initial position of the player.
         *        - x: The x-coordinate in the game world.
         *        - y: The y-coordinate in the game world.
         *        - z: The z-coordinate (depth) in the game world.
         * @param vel The initial velocity of the player.
         *        - x: Horizontal velocity of the player.
         *        - y: Vertical velocity of the player.
         * @param size The size of the player.
         *        - width: The width of the player.
         *        - height: The height of the player.
         */
        Player(
                rtype::ecs::EntityManager &entityManager,
                rtype::ecs::ComponentManager &componentManager,
                rtype::components::Position pos,
                rtype::components::Velocity vel,
                rtype::components::Size size,
                const rtype::components::NetworkConnection& network,
                rtype::components::NetId = { false },
                rtype::components::Speed speed = {200}
        );

        /**
         * @brief Default destructor.
         *
         * Cleans up resources associated with the Player object.
         */
        ~Player() = default;

        /**
         * @brief Gets the unique identifier of the player entity.
         *
         * @return The unique identifier of the player entity.
         */
        [[nodiscard]] size_t getId() const { return _id; };

    void move(rtype::components::Position pos);

    private:
        /**
         * @brief The unique identifier for the player entity.
         */
        size_t _id;

#ifdef RTYPE_IS_CLIENT
        components::InputHandler _inputs;
#endif
    };
}
