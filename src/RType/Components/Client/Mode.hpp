/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Mode.hpp
*/


#pragma once
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

struct Style
{
    bool none = false;
    bool titleBar = false;
    bool resize = false;
    bool close = false;
    bool fullScreen = false;
    bool defaults = true;
    uint32_t style = 0;
};

namespace rtype::components {
    struct Mode : public IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            style.none = value["none"] || false;
            style.titleBar = value["titleBar"] || false;
            style.resize = value["resize"] || false;
            style.close = value["close"] || false;
            style.fullScreen = value["fullscreen"] || false;
            style.defaults = value["defaults"] || true;
            style.style = style.none * sf::Style::None
                + style.titleBar * sf::Style::Titlebar
                + style.resize * sf::Style::Resize
                + style.close * sf::Style::Close
                + style.fullScreen * sf::Style::Fullscreen
                + style.defaults * sf::Style::Default;

        }
        Style style;
    };
}
