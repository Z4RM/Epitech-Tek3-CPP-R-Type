/*
** EPITECH PROJECT, 2025
** RType
** File description:
**
*/

#pragma once
#ifndef RTYPE_IS_CLIENT
namespace rtype::components {
    struct Health {
        int value;
        int max;
        std::chrono::steady_clock::time_point _elapsedDamage = std::chrono::steady_clock::now();
        bool colisionState = false;

        void takeDamage(int amount) {
            value -= amount;

            if (value < 0)
                value = 0;
            _elapsedDamage = std::chrono::steady_clock::now();
        }

        void setHealth(int amount) {
            value = amount;
            if (value < 0)
                value = 0;
        }
    };
}
#else
#include <SFML/Graphics.hpp>
namespace rtype::components {
    struct Health {
        int value;
        int max;
        sf::RectangleShape bgBar;
        sf::RectangleShape healthBar;
        bool created = false;
        std::chrono::steady_clock::time_point _elapsedDamage = std::chrono::steady_clock::now();
        bool colisionState = false;

        Health(int maxHealth, sf::Vector2f position, Size entitySize)
            : value(maxHealth), max(maxHealth) {
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

        void takeDamage(int amount) {
            this->value -= amount;

            if (value < 0)
                value = 0;

            auto size = healthBar.getSize();

            float sizeX = this->value * 50 / this->max;

            if (sizeX < 0)
                sizeX = 0;

            healthBar.setSize({sizeX, size.y});
            _elapsedDamage = std::chrono::steady_clock::now();
        }

        void setHealth(int amount) {
            this->value = amount;

            if (value < 0)
                value = 0;

            auto size = healthBar.getSize();

            float sizeX = this->value * 50 / this->max;

            if (sizeX < 0)
                sizeX = 0;

            healthBar.setSize({sizeX, size.y});
        }
    };
}
#endif
