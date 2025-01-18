/*
** EPITECH PROJECT, 2025
** RType
** File description:
** sftext component
*/

#pragma once
#include <SFML/Graphics/Text.hpp>

namespace rtype::components {
    struct SfText {
        sf::Text text;
        std::shared_ptr<sf::Font> font;
        SfText(std::string str, std::string fontPath, sf::Color color, int charSize, sf::Vector2f position) {
            font = std::make_shared<sf::Font>();
            if (!font->loadFromFile(fontPath)) {
                throw std::runtime_error("Failed to load font from file: " + fontPath);
            }
            text.setFont(*font);
            text.setString(str);
            text.setCharacterSize(charSize);
            text.setFillColor(color);
            text.setStyle(sf::Text::Bold);
            text.setPosition(position);
        }
        ~SfText() = default;
    };
}
