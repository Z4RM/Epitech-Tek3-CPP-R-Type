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

#ifndef RTYPE_IS_CLIENT
rtype::RType::RType(unsigned short port) : _port(port) {}
#else
rtype::RType::RType(unsigned short port) : _port(port), _client(this) {}
#endif

int rtype::RType::_run() {
    ecs::EntityManager entityManager;
    ecs::ComponentManager componentManager;
    ecs::SystemManager systemManager(entityManager, componentManager);

    size_t rtype = entityManager.createEntity();
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

    components::Sprite sprite2 = {{100, 100, 0}, {33, 17}, "assets/sprites/players.gif", {0}};
    rtype::entities::Player player(
            entityManager,
            componentManager,
            {0, 0, 0},
            {0, 0, 0},
            {64, 64},
            sprite2,
            {"", 0, 0},
            { false }
    );

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
    rtype::entities::Player player(
        entityManager,
        componentManager,
        {0, 0, 0},
        {0, 0, 0},
        {64, 64}
        );

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
    while (_running()) {
        systemManager.updateSystems();
#ifdef RTYPE_IS_CLIENT
        //_client.iteration();
#endif
    }
    return 0;
}

bool rtype::RType::_running() {
#ifdef RTYPE_IS_SERVER
    return true; // TODO
#endif
#ifdef RTYPE_IS_CLIENT
    return _client.running();
#endif
}
