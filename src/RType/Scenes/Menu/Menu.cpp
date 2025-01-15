/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#include "Menu.hpp"

#include "RType/Entities/Image.hpp"

void rtype::scenes::Menu::load() {
    components::Sprite bg;
    bg.pos = {0, 0};
    bg.size = {525, 186};
    bg.priority = { 0 };
    bg.path = "./assets/logo.png";

    entities::Image backgroundImage(this->_componentManager, this->_entityManager, bg);
    this->registerEntity(backgroundImage);
}
