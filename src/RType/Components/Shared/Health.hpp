/*
** EPITECH PROJECT, 2025
** RType
** File description:
**
*/

#pragma once

namespace rtype::components {
#ifndef RTYPE_IS_CLIENT
    struct Health {
        int value;
        int max;

        void takeDamage(int amount) {
            value -= amount;

            if (value < 0)
                value = 0;
        }

        void setHealth(int amount) {
            value = amount;
            if (value < 0)
                value = 0;
        }
    };
#else
#include <SFML/Graphics/RectangleShape.hpp>
    struct Health {
        int value;
        int max;
        sf::RectangleShape bgBar;
        sf::RectangleShape healthBar;
        bool created = false;

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
#endif

}
