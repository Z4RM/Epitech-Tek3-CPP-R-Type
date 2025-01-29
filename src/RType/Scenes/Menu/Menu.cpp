/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Menu scene implementation
*/

#include "Menu.hpp"

#include "RType/Components/Shared/MenuState.hpp"
#include "RType/Entities/PlayerCounter.hpp"

#ifdef RTYPE_IS_CLIENT

#include "RType/Entities/Button.hpp"
#include "RType/Entities/Image.hpp"
#include "RType/Components/Client/OnClick.hpp"
#include "RType/Components/Shared/GameState.hpp"

void rtype::scenes::Menu::load() {
    components::Sprite logo;
    logo.pos = {120, 0};
    logo.size = {-1, -1};
    logo.priority = {0};
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
        for (auto &entity: _entityManager.getEntities()) {
            auto gameState = _componentManager.getComponent<components::GameState>(entity);
            if (gameState) {
                gameState->isStarted = true;
                _componentManager.addComponent<components::GameState>(entity, *gameState);
            }
        }
    };
    components::SfText startButtonText("START", "./assets/fonts/Starborn.ttf", sf::Color::White, 50, {300, 300});
    entities::Button startButton(_componentManager, _entityManager, onClick, startButtonText);

    entities::PlayerCounter playerCounter(_componentManager, _entityManager, {250, 400});

    this->registerEntity(playerCounter);
    this->registerEntity(startButton);
    this->registerEntity(backgroundImage);
    this->registerEntity(logoImage);

    unsigned int menuSateEntity = _entityManager.createEntity();
    components::MenuState state = { 0 };

    _componentManager.addComponent<components::MenuState>(menuSateEntity, state);
    AScene::load();
}

#else
void rtype::scenes::Menu::load() {
    unsigned int menuSateEntity = _entityManager.createEntity();
    components::MenuState state = { 0 };

    _componentManager.addComponent<components::MenuState>(menuSateEntity, state);
    AScene::load();
}
#endif
