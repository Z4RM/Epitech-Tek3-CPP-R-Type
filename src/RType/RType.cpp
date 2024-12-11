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
#include "ModeManager/ModeManager.hpp"

int rtype::RType::run() {
    Config::initialize();

    ecs::EntityManager entityManager;
    ecs::ComponentManager componentManager;
    ecs::SystemManager systemManager(entityManager, componentManager);

    size_t rtype = entityManager.createEntity();
    componentManager.addComponent<IsActive>(rtype, {true});
    #ifdef RTYPE_IS_CLIENT
        systemManager.addSystem(rtype::systems::RenderWindowSys::createWindow);
        String title;
        title.s = "RType";
        rtype::components::RWindow renderWindow;
        rtype::components::Mode mode;
        mode.style.style = sf::Style::Default;
        Sprite sprite1 = {{0, 0, 0}, {800, 600}, "sprites/background.jpg", {-1}};
        rtype::components::Window window(
            entityManager,
            componentManager,
            {800, 600},
            {"RType"},
            renderWindow,
            mode,
            sprite1
            );
    systemManager.addSystem(rtype::systems::RenderWindowSys::render);
    #endif

    Sprite sprite2 = {{100, 100, 0}, {33, 17}, "sprites/players.gif", {0}};
    rtype::components::Player player(
        entityManager,
        componentManager,
        {0, 0, 0},
        {0, 0, 0},
        {64, 64},
        sprite2,
        {"", 0, 0}
        );


    while (componentManager.getComponent<IsActive>(rtype)->active) {
        systemManager.updateSystems();
    }
    return 0;
}
