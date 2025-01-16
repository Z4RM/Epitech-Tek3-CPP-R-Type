/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Waves.hpp
*/


#pragma once
#include "IComponent.hpp"
#include <iostream>

#include "RType/Components/Client/Sprite.hpp"

namespace rtype::components {

    /**
     * @struct Movement
     * @brief Represents movement data for an entity, including movement type.
     *
     * The `Movement` component defines the type of movement an entity uses,
     * such as "linear" or "circular".
     */
    struct Movement : public IComponent {
        /**
         * @brief Initializes the `Movement` component with data from a JSON object.
         *
         * This method assigns the movement type to the `type` attribute.
         *
         * @param value A JSON object containing the movement type.
         * The object must include a key for `type`.
         */
        void create(nlohmann::basic_json<> &value) override {
            type = value;
        }

        /**
         * @brief The type of movement, such as "linear", "circular", etc.
         */
        std::string type;
    };

    /**
     * @struct Projectile
     * @brief Represents data for a projectile, including its type, damage, and speed.
     *
     * The `Projectile` component defines the properties of a projectile, including
     * its damage value, speed, and type (e.g., "laser", "missile").
     */
    struct Projectile : public IComponent {
        /**
         * @brief Initializes the `Projectile` component with data from a JSON object.
         *
         * This method assigns values for type, damage, and speed to the respective
         * attributes of the `Projectile` component.
         *
         * @param value A JSON object containing projectile data.
         * The object must include keys for `type`, `damage`, and `speed`.
         */
        void create(nlohmann::basic_json<> &value) override {
            type = value["type"];
            damage = value["damage"];
            speed = value["speed"];
        }

        /**
         * @brief The type of the projectile (e.g., "laser", "missile").
         */
        std::string type;

        /**
         * @brief The damage value of the projectile.
         */
        int damage;

        /**
         * @brief The speed of the projectile.
         */
        int speed;
    };

    /**
     * @struct Enemy
     * @brief Represents an enemy entity, including its movement, health, and projectile data.
     *
     * The `Enemy` component defines the properties of an enemy entity in the game, including
     * movement pattern, spawn position, health, and associated projectile data.
     */
    struct Enemy : public IComponent {
        /**
         * @brief Initializes the `Enemy` component with data from a JSON object.
         *
         * This method assigns values for various attributes such as type, name, movement pattern,
         * health, and others.
         *
         * @param value A JSON object containing enemy data.
         */
        void create(nlohmann::basic_json<> &value) override {
            type = value["type"];
            name = value["type"];
            spawn_position.create(value["spawn_position"]);
            spawn_time = value["spawn_time"];
            movement_pattern.create(value["movement_pattern"]);
            speed = value["speed"];
            if (!value["fire_rate"].is_null()) {
                fire_rate = value["fire_rate"];
            }
            if (!value["explosion_damage"].is_null()) {
                explosion_damage = value["explosion_damage"];
            }
            health = value["health"];
            projectile.create(value["projectile"]);

#ifdef RTYPE_IS_CLIENT
            sprite.create(value["sprite"]);
#endif
            data = value;
            time = std::clock();
            velocity = Velocity();
        }

        /**
         * @brief The data for the enemy, including type, name, spawn position, etc.
         */
        nlohmann::basic_json<> data;

        /**
         * @brief The type of enemy.
         */
        std::string type;

        /**
         * @brief The name of the enemy.
         */
        std::string name;

        /**
         * @brief The spawn position of the enemy.
         */
        Position spawn_position;

        /**
         * @brief The time when the enemy spawns.
         */
        float spawn_time;

        /**
         * @brief The movement pattern for the enemy.
         */
        Movement movement_pattern;

        /**
         * @brief The speed of the enemy.
         */
        int speed;

        /**
         * @brief The fire rate of the enemy.
         */
        int fire_rate;

        /**
         * @brief The explosion damage dealt by the enemy.
         */
        int explosion_damage;

        /**
         * @brief The projectile data associated with the enemy.
         */
        Projectile projectile;

        /**
         * @brief The health of the enemy.
         */
        int health;

        /**
         * @brief The velocity of the enemy.
         */
        Velocity velocity;

        /**
         * @brief The time when the enemy was created.
         */
        std::clock_t time;

#ifdef RTYPE_IS_CLIENT
        /**
         * @brief The sprite associated with the enemy.
         */
        Sprite sprite;
#endif
    };

    /**
     * @struct Enemies
     * @brief Represents a collection of enemies in the game.
     *
     * The `Enemies` component stores multiple `Enemy` entities, enabling the
     * management of enemy groups and their behaviors in the game.
     */
    struct Enemies : public IComponent {
        /**
         * @brief Initializes the `Enemies` component with data from a JSON object.
         *
         * This method creates multiple `Enemy` components based on the data in the
         * provided JSON object.
         *
         * @param value A JSON object containing a list of enemies.
         */
        void create(nlohmann::basic_json<> &value) override {
            for (const auto &enemy: value.items()) {
                Enemy enemyComponent;
                enemyComponent.create(enemy.value());
                enemies.push_back(enemyComponent);
            }
        }

        /**
         * @brief A collection of `Enemy` components.
         */
        std::deque<Enemy> enemies;
    };

    /**
     * @struct Waves
     * @brief Represents a wave of enemies in the game.
     *
     * The `Waves` component defines a collection of enemies that spawn as part of a wave,
     * including the wave ID and a list of enemies in that wave.
     */
    struct Waves : public IComponent {
        /**
         * @brief Initializes the `Waves` component with data from a JSON object.
         *
         * This method processes the data for each wave, including enemy data and wave ID.
         *
         * @param value A JSON object containing wave data.
         */
        void create(nlohmann::basic_json<> &value) override {
            data = value;

            for (const auto &wave: data.items()) {
                for (const auto &enemy: wave.value().items()) {
                    if (enemy.key() == "wave_id") {
                        id = enemy.value();
                        continue;
                    }
                    enemies.create(enemy.value());
                }
            }
        }

        /**
         * @brief The raw JSON data for the wave.
         */
        nlohmann::basic_json<> data;

        /**
         * @brief The ID of the wave.
         */
        int id;

        /**
         * @brief A collection of `Enemy` components for this wave.
         */
        Enemies enemies;
    };
}
