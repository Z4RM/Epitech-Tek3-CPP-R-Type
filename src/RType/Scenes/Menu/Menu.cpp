/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#include "Menu.hpp"

#include "RType/Entities/Image.hpp"

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
    this->registerEntity(backgroundImage);
    this->registerEntity(logoImage);

    AScene::load();
#endif
}
