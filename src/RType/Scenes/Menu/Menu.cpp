/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Menu scene implementation
*/

#include "Menu.hpp"

#include <spdlog/spdlog.h>

#include "Network/Packets/Descriptors/PacketStartGame/PacketStartGame.hpp"
#include "Network/TCPNetwork/TCPNetwork.hpp"
#include "RType/Components/Shared/Counter.hpp"
#include "RType/Components/Shared/MenuState.hpp"
#include "RType/Config/Config.hpp"
#include "RType/Entities/Game.hpp"
#include "RType/Entities/PlayerCounter.hpp"
#include "RType/Levels/LevelManager.hpp"

#ifdef RTYPE_IS_CLIENT

#include "RType/Entities/Button.hpp"
#include "RType/Entities/Image.hpp"
#include "Components.hpp"
#include "Systems.hpp"

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
        network::TCPNetwork &network = network::TCPNetwork::getInstance();

        for (auto &entity: _entityManager.getEntities()) {
            auto gameState = _componentManager.getComponent<components::GameState>(entity);
            if (gameState) {
                gameState->isStarted = true;
                _componentManager.addComponent<components::GameState>(entity, *gameState, _entityManager);
                break;
            }
        }
        for (const auto &entity: _entityManager.getEntities()) {
            auto levelCounter = _componentManager.getComponent<components::Counter>(entity);

            if (levelCounter && levelCounter->name == "level") {
                if (network.getStarted())
                    network.sendPacket(network::PacketStartGame(levelCounter->getCount()));
                break;
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
    components::MenuState state = {0};

    unsigned int levelSelectorEntity = _entityManager.createEntity();
    std::string levelCounterName = "level";
    components::Counter count(1, 8, levelCounterName, {260, 475}, 35, false);
    _componentManager.addComponent<components::Counter>(levelSelectorEntity, count, _entityManager);


    components::OnClick changeLevel;
    changeLevel.fn = [this]() {
        for (const auto &entity: _entityManager.getEntities()) {
            auto counter = _componentManager.getComponent<components::Counter>(entity);

            if (counter && counter->name == "level") {
                int count = counter->getCount();
                std::vector<std::shared_ptr<levels::Level>> &levels = levels::LevelManager::getInstance().getLevels();

                for (int i = 0; i < levels.size(); i++) {
                    if (levels[i]->getNumber() == count) {
                        if (i + 1 < levels.size()) {
                            counter->update(levels[i + 1]->getNumber());
                        } else {
                            counter->update(levels[0]->getNumber());
                        }
                    }
                }
                _componentManager.addComponent<components::Counter>(entity, *counter, _entityManager);
            }
        }
    };
    components::SfText changeLevelText("->", "./assets/fonts/Starborn.ttf", sf::Color::White, 50, {475, 465});
    entities::Button changeLevelButton(_componentManager, _entityManager, changeLevel, changeLevelText);

    this->registerEntity(levelSelectorEntity);
    this->registerEntity(menuSateEntity);
    this->registerEntity(changeLevelButton);

    _componentManager.addComponent<components::MenuState>(menuSateEntity, state, _entityManager);

    network::TCPNetwork &network = network::TCPNetwork::getInstance(Config::getInstance().getNetwork().server.port);
    if (network.getStarted()) {
        network::PacketConnect packet;
        network.sendPacket(packet);
    }

    systems::Sound::createMusic("assets/sounds/musics/menu.mp3", _componentManager, menuSateEntity, true, 8.25);

    AScene::load();
}

#else
void rtype::scenes::Menu::load() {
    unsigned int menuSateEntity = _entityManager.createEntity();
    components::MenuState state = { 0 };

    _componentManager.addComponent<components::MenuState>(menuSateEntity, state, _entityManager);

    this->registerEntity(menuSateEntity);

    AScene::load();
}
#endif
