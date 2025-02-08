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
#include "Systems/LevelRunner.hpp"
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

#else
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

    void rtype::RType::loadLevels() {
        std::string folder = "./assets/levels"; //TODO: make this configurable in the .ini file

        if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
            spdlog::error("Levels folder: {} does not exist", folder);
            throw std::runtime_error("Failing to load levels");
        }

       for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                spdlog::info("loading level: {}", static_cast<std::string>(entry.path().filename()));

                std::ifstream file(entry.path());
                if (!file.is_open()) {
                    spdlog::error("Error while opening: {}", static_cast<std::string>(entry.path()));
                    continue;
                }

                try {
                    nlohmann::json jsonData;
                    file >> jsonData;

                    if (jsonData.contains("id") && jsonData.contains("duration") && jsonData.contains("spawns")) {
                        int id = jsonData["id"].get<int>();
                        int duration = jsonData["duration"].get<int>();
                        levels::LevelBuilder builder;

                        builder.setDuration(duration);
                        builder.setNumber(id);
                        for (auto &spawn : jsonData["spawns"]) {
                            models::SpawnPoint spawnPoint;

                            if (spawn.contains("time") && spawn.contains("enemies")) {
                                int time = spawn["time"].get<int>();

                                spawnPoint.time = time;
                                for (auto &enemySpawn : spawn["enemies"]) {
                                    if (enemySpawn.contains("quantity") && enemySpawn.contains("type")) {
                                        int quantity = enemySpawn["quantity"].get<int>();
                                        auto type = static_cast<models::EEnemyType>(enemySpawn["type"].get<int>());
                                        models::EnemySpawn enemy = {quantity, type};

                                        spawnPoint.enemies.emplace_back(enemy);
                                    }
                                }
                            }
                            builder.addSpawnPoint(spawnPoint);
                        }
                        levels::LevelManager::getInstance().registerLevel(std::make_unique<levels::Level>(builder.build()));
                        spdlog::debug("Level {} registered", id);
                    }
                } catch (const nlohmann::json::parse_error& e) {
                    spdlog::error("Parsing error in {} : {}", static_cast<std::string>(entry.path()), e.what());
                }
            }
        }
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
    loadLevels();
    systemManager.addSystem(systems::LevelRunner::process);
    levels::LevelManager::getInstance().changeLevel(2);
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
    }
}
