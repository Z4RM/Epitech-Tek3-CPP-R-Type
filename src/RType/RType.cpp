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
#include "Levels/LevelBuilder.hpp"
#include "Levels/LevelManager.hpp"
#include "Scenes/Game/Game.hpp"
#include "Scenes/Menu/Menu.hpp"
#include "Systems/AnimationProjectile.hpp"
#include "Systems/MonsterSpawner.hpp"
#include "Systems/Network/Network.hpp"
#ifdef RTYPE_IS_CLIENT
#include "Entities/Window.hpp"
#endif

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

int rtype::RType::run() {
    if (!Config::initialize())
        return 84;

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
    rtype::entities::Window(
            entityManager,
            componentManager,
            {800, 600},
            {"RType"},
            renderWindow,
            mode
    );
    systemManager.addSystem(rtype::systems::RenderWindowSys::render);
    systemManager.addSystem(rtype::systems::UpdateProjectilesSystem::updateProjectiles);
#else
    systemManager.addSystem(systems::MonsterSpawner::spawnMonster);
#endif
    systemManager.addSystem(rtype::systems::Movement::move);
    systemManager.addSystem(rtype::systems::Network::udpProcess);
    systemManager.addSystem(rtype::systems::Network::tcpProcess);

    std::shared_ptr<scenes::Menu> menu = std::make_shared<scenes::Menu>(entityManager, componentManager);
    sceneManager.registerScene(0, std::move(menu));

    std::shared_ptr<scenes::Game> game = std::make_shared<scenes::Game>(entityManager, componentManager);
    sceneManager.registerScene(1, std::move(game));

    //TODO: put this component in the game scene instead of here
    entities::Game gameSate(componentManager, entityManager);

    levels::LevelBuilder levelBuilder;

    rtype::models::SpawnPoint spawn1{5, {
        {12, models::EEnemyType::BASIC}}
    };

    rtype::models::SpawnPoint spawn2{7, {
            {12, models::EEnemyType::BASIC},
            {5, models::EEnemyType::BASIC}
    }
    };

    levels::Level test = levelBuilder.setDuration(10)
    .setNumber(1)
    .addSpawnPoint(spawn1)
    .addSpawnPoint(spawn2)
    .build();

    levels::LevelManager::getInstance().registerLevel(std::make_shared<levels::Level>(test));
    levels::LevelManager::getInstance().changeLevel(1);

    while (true) {
        for (auto &entity: entityManager.getEntities()) {
            auto run = componentManager.getComponent<components::Running>(entity);
            if (run && !run->running) {
                network::TCPNetwork::getInstance().setStop(true);
                network::UDPNetwork::getInstance().setStop(true);
                spdlog::debug("Program stopped");
                return 0;
            }
        }
        sceneManager.updateCurrentScene(systemManager);
        levels::LevelManager::getInstance().processCurrentLevel();
    }
}
