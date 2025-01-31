/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Game scene
*/

#include "Game.hpp"

#include "RType/Components/Client/SlidingBg.hpp"

#ifdef RTYPE_IS_CLIENT

#include "Components.hpp"
#include "RType/Entities/Image.hpp"

#endif

namespace rtype::scenes {
    void Game::load() {
#ifdef RTYPE_IS_CLIENT
        components::Sprite backgroundSprite = {{0, 0, 0}, {-1, -1}, "assets/sprites/background.png", {-1}};

        entities::Image img(this->_componentManager, this->_entityManager, backgroundSprite, true, true);

        components::Sprite backgroundSprite2 = {{800, 0, 0}, {-1, -1}, "assets/sprites/background.png", {-1}};

        entities::Image img2(this->_componentManager, this->_entityManager, backgroundSprite2, true, true);

        this->registerEntity(img);
#endif
        AScene::load();
    }
}
