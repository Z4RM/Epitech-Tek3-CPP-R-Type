/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Window.hpp
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "ECS.hpp"
#include "Structures.hpp"

struct RWindow
{
    mutable sf::RenderWindow *window;
};

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

struct Mode
{
    sf::VideoMode mode;
    Style style;
};

struct FrameLimit
{
    unsigned int limit = 60;
};

namespace rtype::components
{
    class Window {
    public:
        Window(
            rtype::ecs::EntityManager &entityManager,
            rtype::ecs::ComponentManager& componentManager,
            Size size,
            const String &title,
            const RWindow& renderWindow,
            Mode mode,
            FrameLimit frameLimit,
            Sprite &backgroundSprite
            );

        ~Window() = default;

        [[nodiscard]] size_t getId() const
        {
            return _id;
        }

    private:
        size_t _id;
    };
}