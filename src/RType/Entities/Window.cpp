/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Window.cpp
*/

#include "Window.hpp"

rtype::entities::Window::Window(
    rtype::ecs::EntityManager& entityManager,
    rtype::ecs::ComponentManager& componentManager,
    const components::Size size,
    const components::String& title,
    const RWindow& renderWindow,
    Mode mode
    )
{
    _id = entityManager.createEntity();
    renderWindow.window = std::make_shared<sf::RenderWindow>();
    const uint32_t style = mode.style.none * sf::Style::None
        + mode.style.titleBar * sf::Style::Titlebar
        + mode.style.resize * sf::Style::Resize
        + mode.style.close * sf::Style::Close
        + mode.style.fullScreen * sf::Style::Fullscreen
        + mode.style.defaults * sf::Style::Default;
    mode.style.style = style;
    const int width = static_cast<int>(size.width);
    const int height = static_cast<int>(size.height);
    mode.mode = sf::VideoMode(width, height);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::String>(_id, title);
    componentManager.addComponent<RWindow>(_id, renderWindow);
    componentManager.addComponent<Mode>(_id, mode);
    componentManager.addComponent<components::Created>(_id, components::Created{false});
}
