/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Texture manager
*/

#include "TextureManager.hpp"

#include <spdlog/spdlog.h>

namespace rtype {
    std::shared_ptr<sf::Texture> TextureManager::getTexture(std::string key) {
        if (this->_textures.find(key) != this->_textures.end()) {
            return this->_textures[key];
        }
        spdlog::error("Unknow texture");
        return nullptr;
    }

    void TextureManager::registerTexture(std::string key, std::string path, sf::IntRect rect) {
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

        texture->loadFromFile(path, rect);
        this->_textures[key] = texture;
    }

    void TextureManager::registerTexture(std::string key, std::string path) {
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

        texture->loadFromFile(path);
        this->_textures[key] = texture;
    }


}
