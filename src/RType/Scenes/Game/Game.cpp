/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Game scene
*/

#include "Game.hpp"
#include "Components.hpp"
#include "RType/Entities/Image.hpp"

namespace rtype::scenes {
    void Game::load() {
    #ifdef RTYPE_IS_CLIENT

        components::Sprite backgroundSprite = {{0, 0, 0}, {-1, -1}, "assets/sprites/background.jpg", {-1}};
        entities::Image img(this->_componentManager, this->_entityManager, backgroundSprite, true);
        this->registerEntity(img);
    #endif
        AScene::load();
    }
}
