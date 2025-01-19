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
#include "ECS/Scene/SceneManager.hpp"
#include "Entities/Game.hpp"
#include "Scenes/Game/Game.hpp"
#include "Scenes/Menu/Menu.hpp"
#include "Systems/AnimationProjectile.hpp"
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

rtype::RType::RType(unsigned short port) : _port(port) {}

int rtype::RType::_run() {
    ecs::EntityManager entityManager;
    ecs::ComponentManager componentManager;
    ecs::SystemManager systemManager;
    ecs::SceneManager &sceneManager = ecs::SceneManager::getInstance();

    size_t rtype = entityManager.createEntity();

    componentManager.addComponent<components::Running>(rtype, {true});

#ifdef RTYPE_IS_CLIENT
    systemManager.addSystem(rtype::systems::RenderWindowSys::createWindow);
    components::String title;
    title.s = "RType";
    rtype::entities::RWindow renderWindow{};
    rtype::entities::Mode mode;
    mode.style.style = sf::Style::Default;
    rtype::entities::Window window(
            entityManager,
            componentManager,
            {800, 600},
            {"RType"},
            renderWindow,
            mode
    );
    systemManager.addSystem(rtype::systems::RenderWindowSys::render);
    systemManager.addSystem(rtype::systems::UpdateProjectilesSystem::updateProjectiles);
#endif

    systemManager.addSystem(rtype::systems::Movement::move);
    systemManager.addSystem(rtype::systems::Network::udpProcess);
    systemManager.addSystem(rtype::systems::Network::tcpProcess);

    std::shared_ptr<scenes::Menu> menu = std::make_shared<scenes::Menu>(entityManager, componentManager);
    sceneManager.registerScene(0, std::move(menu));

    std::shared_ptr<scenes::Game> game = std::make_shared<scenes::Game>(entityManager, componentManager);
    sceneManager.registerScene(1, std::move(game));

    entities::Game gameSate(componentManager, entityManager);


    while (true) {
        int runningStoppedCount = 0;

        for (auto &entity: entityManager.getEntities()) {
            if (entity == rtype)
                continue;
            auto run = componentManager.getComponent<components::Running>(entity);
            if (run && !run->running) {
                runningStoppedCount += 1;
            }
        }

        if (runningStoppedCount > 0) {
            spdlog::warn("stop");
            break;
        }
        sceneManager.updateCurrentScene(systemManager);
    }
    return 0;
}
