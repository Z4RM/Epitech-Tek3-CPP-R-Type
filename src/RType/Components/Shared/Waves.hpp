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
    struct Movement : public IComponent {
        void create(nlohmann::basic_json<> &value) override {
            type = value;
        }

        std::string type;
    };

    struct Projectile : public IComponent {
        void create(nlohmann::basic_json<> &value) override {
            type = value["type"];
            damage = value["damage"];
            speed = value["speed"];
        }

        std::string type;
        int damage;
        int speed;
    };

    struct Enemy : public IComponent {
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
        }

        nlohmann::basic_json<> data;
        std::string type;
        std::string name;
        Position spawn_position;
        float spawn_time;
        Movement movement_pattern;
        int speed;
        int fire_rate;
        int explosion_damage;
        Projectile projectile;
        int health;
        std::clock_t time;
#ifdef RTYPE_IS_CLIENT
        Sprite sprite;
#endif
    };

    struct Enemies : public IComponent {
        void create(nlohmann::basic_json<> &value) override {
            for (const auto& enemy : value.items()) {
                Enemy enemyComponent;
                enemyComponent.create(enemy.value());
                enemies.push_back(enemyComponent);
            }
        }

        std::deque<Enemy> enemies;
    };

    struct Waves : public IComponent {
        void create(nlohmann::basic_json<> &value) override {
            data = value;

            for (const auto& wave : data.items()) {
                std::cout << "Wave: " << wave.key() << std::endl;
                for (const auto& enemy : wave.value().items()) {
                    if (enemy.key() == "wave_id") {
                        id = enemy.value();
                        continue;
                    }
                    enemies.create(enemy.value());
                }
            }
        }

        nlohmann::basic_json<> data;
        int id;
        Enemies enemies;
    };
}
