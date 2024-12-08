/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game.hpp
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace rtype {
    class Game {
    public:
        Game(int width, int height, const std::string& title);
        ~Game();
        void run();

    private:
        void processEvents();
        void update();
        void render();

        sf::RenderWindow _window;
        sf::Texture _backgroundTexture;
        sf::Sprite _backgroundSprite;
    };

}

#endif //GAME_HPP
