/*
** EPITECH PROJECT, 2025
** RType
** File description:
** RWindow.hpp
*/


#pragma once
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

namespace rtype::components {
    struct RWindow : public IComponent
    {
        void create(nlohmann::basic_json<> &value) {
            window = new sf::RenderWindow();
            created.isCreate = false;
        }
        mutable sf::RenderWindow *window;
        Created created;
    };
}
