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
#include "Scenes/Lose/Lose.hpp"
#include "Scenes/Win/Win.hpp"
#ifdef RTYPE_IS_CLIENT
#include "Entities/Window.hpp"
#include "TextureManager/TextureManager.hpp"
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
                spdlog::info("loading level: {}", entry.path().filename().string());

                std::ifstream file(entry.path());
                if (!file.is_open()) {
                    spdlog::error("Error while opening: {}", entry.path().string());
                    continue;
                }

                try {
                    nlohmann::json jsonData;
                    file >> jsonData;

                    if (jsonData.contains("id") && jsonData.contains("duration") && jsonData.contains("spawns")) {
                        int id = jsonData["id"].get<int>();

                        if (id < 0)
                            continue;
                        int duration = jsonData["duration"].get<int>();
                        levels::LevelBuilder builder;

                        builder.setDuration(duration);
                        builder.setNumber(id);
                        for (auto &spawn : jsonData["spawns"]) {
                            models::SpawnPoint spawnPoint;

                            if (spawn.contains("time")) {
                                int time = spawn["time"].get<int>();

                                spawnPoint.time = time;
                                if (spawn.contains("enemies")) {
                                    for (auto &enemySpawn : spawn["enemies"]) {
                                        if (enemySpawn.contains("quantity") && enemySpawn.contains("type")) {
                                            int quantity = enemySpawn["quantity"].get<int>();
                                            auto type = static_cast<models::EEnemyType>(enemySpawn["type"].get<int>());
                                            models::EnemySpawn enemy = {quantity, type};

                                            spawnPoint.enemies.emplace_back(enemy);
                                        }
                                    }
                                }
                                if (spawn.contains("bonuses")) {
                                    for (auto &bonus : spawn["bonuses"]) {
                                        auto bonusType = static_cast<models::EBonusType>(bonus.get<int>());
                                        spawnPoint.bonuses.emplace_back(bonusType);
                                    }
                                }
                            }
                            builder.addSpawnPoint(spawnPoint);
                        }
                        levels::LevelManager::getInstance().registerLevel(std::make_unique<levels::Level>(builder.build()));
                        spdlog::debug("Level {} registered", id);
                    }
                } catch (const nlohmann::json::parse_error& e) {
                    spdlog::error("Parsing error in {} : {}", entry.path().string(), e.what());
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

    componentManager.addComponent<components::Running>(rtype, {true}, entityManager);

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
    TextureManager::getInstance().registerTexture("player", "assets/sprites/players.gif", {0, 3, 32, 14});
    TextureManager::getInstance().registerTexture("player2", "assets/sprites/players.gif", {0, 20, 32, 14});
    TextureManager::getInstance().registerTexture("player3", "assets/sprites/players.gif", {0, 37, 32, 14});
    TextureManager::getInstance().registerTexture("player4", "assets/sprites/players.gif", {0, 54, 32, 14});
    TextureManager::getInstance().registerTexture("enemy", "assets/sprites/enemy.gif", {0, 0, 33, 36});
    TextureManager::getInstance().registerTexture("projectile", "assets/sprites/projectile/player-shots.gif");
    TextureManager::getInstance().registerTexture("super_projectile", "assets/sprites/projectile/player-shots-charged.gif");
    TextureManager::getInstance().registerTexture("force", "assets/sprites/force.gif", {162, 411, 32, 20});
    TextureManager::getInstance().registerTexture("boss_turret", "assets/sprites/boss_turret.gif", {1, 1, 592, 176});
    TextureManager::getInstance().registerTexture("turret", "assets/sprites/boss_turret.gif", {153, 908, 32, 15});
    TextureManager::getInstance().registerTexture("enemy_projectile", "assets/sprites/enemy-projectile.gif");
    TextureManager::getInstance().registerTexture("cornus", "assets/sprites/enemy-cornus.gif", {223, 140, 62, 46});
    TextureManager::getInstance().registerTexture("crabus", "assets/sprites/enemy-crabus.gif", {135, 36, 29, 28});
    TextureManager::getInstance().registerTexture("pablitos", "assets/sprites/enemy-pablitos.gif", {1, 20, 32, 28});
    TextureManager::getInstance().registerTexture("boss_heart", "assets/sprites/boss-heart.gif", {2, 292, 256, 142});
    TextureManager::getInstance().registerTexture("shield", "assets/sprites/shield.gif", {117, 198, 40, 34});

    systemManager.addSystem(rtype::systems::RenderWindowSys::render);
    systemManager.addSystem(rtype::systems::Sound::play);
    systemManager.addSystem(rtype::systems::UpdateProjectilesSystem::updateProjectiles);
#else
    loadLevels();
    systemManager.addSystem(systems::LevelRunner::process);
#endif
    systemManager.addSystem(rtype::systems::Movement::move);
    systemManager.addSystem(rtype::systems::Network::udpProcess);
    systemManager.addSystem(rtype::systems::Network::tcpProcess);

    std::shared_ptr<scenes::Menu> menu = std::make_shared<scenes::Menu>(entityManager, componentManager);
    sceneManager.registerScene(0, std::move(menu));

    std::shared_ptr<scenes::Game> game = std::make_shared<scenes::Game>(entityManager, componentManager);
    sceneManager.registerScene(1, std::move(game));

    std::shared_ptr<scenes::Lose> lose = std::make_shared<scenes::Lose>(entityManager, componentManager);
    sceneManager.registerScene(2, std::move(lose));

    std::shared_ptr<scenes::Win> win = std::make_shared<scenes::Win>(entityManager, componentManager);
    sceneManager.registerScene(3, std::move(win));

    //TODO: put this component in the game scene instead of here
    entities::Game gameSate(componentManager, entityManager);

    sceneManager.changeScene(0);

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
