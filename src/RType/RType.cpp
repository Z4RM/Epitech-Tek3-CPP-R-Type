/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RType.cpp
*/

#include "RType/Config/Config.hpp"
#include "RType.hpp"
#include "ECS.hpp"
#include "Systems.hpp"
#include "Components/Player.hpp"

int rtype::RType::run() {
    Config::initialize();

    ecs::EntityManager entityManager;
    ecs::ComponentManager componentManager;
    ecs::SystemManager systemManager(entityManager, componentManager);

    systemManager.addSystem(rtype::systems::RenderWindowSys::createWindow);
    systemManager.addSystem(rtype::systems::RenderWindowSys::render);

    String title;
    title.s = "RType";
    RWindow renderWindow;
    Mode mode;
    mode.style.style = sf::Style::Default;
    Sprite sprite1 = {{0, 0, 0}, {800, 600}, "sprites/background.jpg", {-1}};
    rtype::components::Window window(
        entityManager,
        componentManager,
        {800, 600},
        {"RType"},
        renderWindow,
        mode,
        {60},
        sprite1
        );

    Sprite sprite2 = {{0, 0, 0}, {64, 64}, "sprites/r-typesheet42.gif", {0}};
    rtype::components::Player player(
        entityManager,
        componentManager,
        {0, 0, 0},
        {0, 0, 0},
        {64, 64},
        sprite2,
        {"", 0, 0}
        );

    while (1) {
        systemManager.updateSystems();
    }
}
