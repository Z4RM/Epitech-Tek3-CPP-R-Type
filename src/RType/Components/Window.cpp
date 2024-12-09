/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Window.cpp
*/

#include "Window.hpp"

rtype::components::Window::Window(
    rtype::ecs::EntityManager& entityManager,
    rtype::ecs::ComponentManager& componentManager,
    const Size size,
    const String& title,
    const RWindow& renderWindow,
    Mode mode,
    const FrameLimit frameLimit,
    Sprite& backgroundSprite
    )
{
    _id = entityManager.createEntity();
    renderWindow.window = new sf::RenderWindow();
    const uint32_t style = mode.style.none * sf::Style::None
        + mode.style.titleBar * sf::Style::Titlebar
        + mode.style.resize * sf::Style::Resize
        + mode.style.close * sf::Style::Close
        + mode.style.fullScreen * sf::Style::Fullscreen
        + mode.style.defaults * sf::Style::Default;
    mode.style.style = style;
    backgroundSprite.texture = new sf::Texture();
    backgroundSprite.sprite = new sf::Sprite();
    backgroundSprite.texture->loadFromFile(backgroundSprite.path);
    backgroundSprite.sprite->setTexture(*backgroundSprite.texture);
    backgroundSprite.sprite->setPosition({0, 0});
    backgroundSprite.sprite->setScale(
        size.width / backgroundSprite.sprite->getGlobalBounds().width,
        size.height / backgroundSprite.sprite->getGlobalBounds().height
        );
    componentManager.addComponent<Size>(_id, size);
    componentManager.addComponent<String>(_id, title);
    componentManager.addComponent<RWindow>(_id, renderWindow);
    componentManager.addComponent<Mode>(_id, mode);
    componentManager.addComponent<FrameLimit>(_id, frameLimit);
    componentManager.addComponent<Sprite>(_id, backgroundSprite);
    componentManager.addComponent<Created>(_id, Created{false});
}
