/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game.cpp
*/

#include "Game.hpp"
#include <iostream>

rtype::Game::Game(int width, int height, const std::string &title) : _window(sf::VideoMode(width, height), title)
{
    _window.setFramerateLimit(60);
    if (!_backgroundTexture.loadFromFile("sprites/background.jpg")) {
        std::cerr << "Error : Impossible to load 'background.jpg'." << std::endl;
        throw std::runtime_error("Texture not found");
    }
    _backgroundSprite.setTexture(_backgroundTexture);
    sf::Vector2u textureSize = _backgroundTexture.getSize();
    sf::Vector2u windowSize = _window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    _backgroundSprite.setScale(scaleX, scaleY);
}

rtype::Game::~Game() {}

void rtype::Game::run()
{
    while (_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void rtype::Game::processEvents() {
    sf::Event event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
    }
}

void rtype::Game::update() {}

void rtype::Game::render() {
    _window.clear();
    _window.draw(_backgroundSprite);
    _window.display();
}
