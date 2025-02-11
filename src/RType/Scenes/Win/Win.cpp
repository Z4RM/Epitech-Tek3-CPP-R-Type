/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Win.cpp
*/


#include "Win.hpp"

#include "ECS/Scene/SceneManager.hpp"
#include "Network/Packets/Descriptors/PacketStartGame/PacketStartGame.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/Components/Shared/MenuState.hpp"
#include "RType/Entities/PlayerCounter.hpp"
#include "RType/Systems/Network/Network.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/Entities/Button.hpp"
#include "RType/Entities/Image.hpp"
#include "RType/Components/Client/OnClick.hpp"

void rtype::scenes::Win::load() {
    components::Sprite win;
    win.pos = {80, 0};
    win.size = {-1, -1};
    win.priority = {0};
    win.path = "./assets/win.png";
    entities::Image winImage(this->_componentManager, this->_entityManager, win);

    components::Sprite bg;
    bg.path = "./assets/backgroundMenu.jpg";
    bg.size = {-1, -1};
    bg.priority = {-1};
    bg.pos = {0, 0};
    entities::Image backgroundImage(this->_componentManager, this->_entityManager, bg, true);

    components::OnClick onClick;
    onClick.fn = [this]() {
        ecs::SceneManager::getInstance().changeScene(0, true);
    };

    components::SfText backHomeButtonText("MAIN MENU", "./assets/fonts/Starborn.ttf", sf::Color::White, 50, {230, 350});
    entities::Button backHomeButton(_componentManager, _entityManager, onClick, backHomeButtonText);

    this->registerEntity(backHomeButton);
    this->registerEntity(backgroundImage);
    this->registerEntity(winImage);

    unsigned int winSateEntity = _entityManager.createEntity();
    components::MenuState state = { 0 };

    _componentManager.addComponent<components::MenuState>(winSateEntity, state, _entityManager);
    AScene::load();
}

#else
void rtype::scenes::Win::load() {
    unsigned int winSateEntity = _entityManager.createEntity();
    components::MenuState state = { 0 };

    _componentManager.addComponent<components::MenuState>(winSateEntity, state, _entityManager);

    this->registerEntity(winSateEntity);
    AScene::load();
}
#endif