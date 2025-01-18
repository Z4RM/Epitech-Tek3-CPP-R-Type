/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Menu scene implementation
*/

#include "Menu.hpp"

#include "ECS/Scene/SceneManager.hpp"
#include "RType/Entities/Image.hpp"
#include "RType/Components/Client/OnClick.hpp"
#include "RType/Components/Shared/GameState.hpp"
#include "RType/Entities/Button.hpp"

void rtype::scenes::Menu::load() {

#ifdef RTYPE_IS_CLIENT
    components::Sprite logo;
    logo.pos = {120, 0};
    logo.size = {-1, -1};
    logo.priority = { 0 };
    logo.path = "./assets/logo.png";
    entities::Image logoImage(this->_componentManager, this->_entityManager, logo);

    components::Sprite bg;
    bg.path = "./assets/backgroundMenu.jpg";
    bg.size = {-1, -1};
    bg.priority = {-1};
    bg.pos = {0, 0};
    entities::Image backgroundImage(this->_componentManager, this->_entityManager, bg, true);

    components::OnClick onClick;
    onClick.fn = [this]() {
        for (auto &entity : _entityManager.getEntities()) {
            auto gameState = _componentManager.getComponent<components::GameState>(entity);
            if (gameState) {
                gameState->isStarted = true;
            }
        }
    };
    components::SfText startButtonText("START", "./assets/fonts/Starborn.ttf", sf::Color::White, 50, {300, 300});
    entities::Button startButton(_componentManager, _entityManager, onClick, startButtonText);

    this->registerEntity(startButton);
    this->registerEntity(backgroundImage);
    this->registerEntity(logoImage);

    AScene::load();
#endif
}
