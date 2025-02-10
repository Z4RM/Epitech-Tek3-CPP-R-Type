/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TextureManager
*/

#pragma once

#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

namespace rtype {
    class TextureManager {
    public:
        static TextureManager &getInstance() {
            static TextureManager instance;

            return instance;
        }

        std::shared_ptr<sf::Texture> getTexture(std::string key);

        void registerTexture(std::string key, std::string path, sf::IntRect rect);
    private:
        std::map<std::string, std::shared_ptr<sf::Texture>> _textures;

    };
}
