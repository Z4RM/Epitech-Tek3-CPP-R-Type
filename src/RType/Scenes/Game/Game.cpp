/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Game scene
*/

#include "Game.hpp"

#include "RType/Components/Client/SlidingBg.hpp"

#ifdef RTYPE_IS_CLIENT

#include "RType/Entities/Image.hpp"
#include "Components.hpp"
#include "Systems.hpp"

#endif

namespace rtype::scenes {
    void Game::load() {
#ifdef RTYPE_IS_CLIENT
        unsigned int gameEntity = _entityManager.createEntity();

        components::Sprite backgroundSprite = {{0, 0, 0}, {-1, -1}, "assets/sprites/background.png", {-1}};

        entities::Image img(this->_componentManager, this->_entityManager, backgroundSprite, true, true);

        components::Sprite backgroundSprite2 = {{800, 0, 0}, {-1, -1}, "assets/sprites/background.png", {-1}};

        entities::Image img2(this->_componentManager, this->_entityManager, backgroundSprite2, true, true);

        this->registerEntity(gameEntity);
        this->registerEntity(img);
        this->registerEntity(img2);

        systems::Sound::createMusic("assets/sounds/musics/game.mp3", _componentManager, gameEntity, true, std::nullopt);
#endif
        AScene::load();
    }
}
