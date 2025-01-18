/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RType.cpp
*/

#include "ModeManager/ModeManager.hpp"
#include "Config/Config.hpp"
#include "ECS.hpp"
#include "Entities/Enemy.hpp"
#include "Entities/Player.hpp"
#include "Systems.hpp"
#include "RType.hpp"
#include "Networks.hpp"
#include "Systems/Network.hpp"
#include "Components.hpp"
#ifdef RTYPE_IS_CLIENT
#include "Entities/Window.hpp"
#endif

int rtype::RType::run() {
    if (!Config::initialize())
        return 84;

    RType rtype(Config::getInstance().getNetwork().server.port);

    return rtype._run();
}

#ifdef RTYPE_IS_CLIENT

void rtype::RType::startServer() {
    if (IS_SERVER) {
        spdlog::warn("Server seems to be already started, do nothing");
        return;
    }
    ENABLE_SERVER();
    // TODO
}

void rtype::RType::stopServer() {
    if (!IS_SERVER) {
        spdlog::warn("Server seems to be already stopped, do nothing");
        return;
    }
    DISABLE_SERVER();
    // TODO
}

#endif

rtype::RType::RType(unsigned short port) : _port(port) {}

int rtype::RType::_run() {
    ecs::EntityManager entityManager;
    ecs::ComponentManager componentManager;
    ecs::SystemManager systemManager(entityManager, componentManager);

    size_t rtype = entityManager.createEntity();

    componentManager.addComponent<components::Running>(rtype, {true});

#ifdef RTYPE_IS_CLIENT
    systemManager.addSystem(rtype::systems::RenderWindowSys::createWindow);
    components::String title;
    title.s = "RType";
    rtype::entities::RWindow renderWindow{};
    rtype::entities::Mode mode;
    mode.style.style = sf::Style::Default;
    components::Sprite sprite1 = {{0, 0, 0}, {800, 600}, "assets/sprites/background.jpg", {-1}};
    rtype::entities::Window window(
            entityManager,
            componentManager,
            {800, 600},
            {"RType"},
            renderWindow,
            mode,
            sprite1
    );
    systemManager.addSystem(rtype::systems::RenderWindowSys::render);

    components::Sprite sprite3 = {{600, 100, 0}, {33, 36}, "assets/sprites/enemy.gif", {1}};
    rtype::entities::Enemy enemy(
        entityManager,
        componentManager,
        {600, 100, 0},
        {0, 0, 0},
        {64, 64},
        sprite3,
        {"", 0, 0}
    );
#else
    rtype::entities::Enemy enemy(
        entityManager,
        componentManager,
        {600, 100, 0},
        {0, 0, 0},
        {64, 64}
    );
#endif

    systemManager.addSystem(rtype::systems::Movement::move);
    systemManager.addSystem(rtype::systems::Network::udpProcess);
    systemManager.addSystem(rtype::systems::Network::tcpProcess);

    while (componentManager.getComponent<components::Running>(rtype)->running) {
        systemManager.updateSystems();
    }
    return 0;
}
