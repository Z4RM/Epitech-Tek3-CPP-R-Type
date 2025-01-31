/*
** EPITECH PROJECT, 2025
** RType
** File description:
**
*/

#pragma once

#ifdef RTYPE_IS_CLIENT
#include <SFML/Graphics.hpp>
#endif

namespace rtype::components {
    struct Health {
        int value;
        int max;

#ifdef RTYPE_IS_CLIENT
        sf::RectangleShape bgBar;
        sf::RectangleShape healthBar;
        bool created = false;
#endif

        std::chrono::steady_clock::time_point _elapsedDamage = std::chrono::steady_clock::now();
        bool collisionState = false;

        explicit Health(int maxHealth) : value(maxHealth), max(maxHealth) {}

#ifdef RTYPE_IS_CLIENT

        Health(int maxHealth, sf::Vector2f position, Size entitySize) : Health(maxHealth) {
            sf::Vector2f size = {static_cast<float>(50), 4};

            position.x += entitySize.width / 5;
            bgBar.setSize(size);
            bgBar.setFillColor(sf::Color::Red);
            bgBar.setPosition(position);

            healthBar.setSize(size);
            healthBar.setFillColor(sf::Color::Green);
            healthBar.setPosition(position);
            created = true;
        }

#endif

        void takeDamage(int amount) {
            value -= amount;

            if (value < 0)
                value = 0;

#ifdef RTYPE_IS_CLIENT
            auto size = healthBar.getSize();
            float sizeX = (float) value * 50 / (float) max;
            if (sizeX < 0)
                sizeX = 0;
            healthBar.setSize({sizeX, size.y});
#endif

            _elapsedDamage = std::chrono::steady_clock::now();
        }

        void setHealth(int amount) {
            value = amount;
            if (value < 0)
                value = 0;

#ifdef RTYPE_IS_CLIENT
            auto size = healthBar.getSize();
            float sizeX = (float) value * 50 / (float) max;
            if (sizeX < 0)
                sizeX = 0;
            healthBar.setSize({sizeX, size.y});
#endif
        }
    };
}
