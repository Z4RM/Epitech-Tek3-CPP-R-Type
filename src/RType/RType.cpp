/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RType.cpp
*/

#include "ModeManager/ModeManager.hpp"
#include "Config/Config.hpp"
#include "ECS.hpp"
#include "Systems.hpp"
#include "RType.hpp"

#include "Components.hpp"
#include "Networks.hpp"
#include "Systems/Network.hpp"

int rtype::RType::run() {
    if (!Config::initialize("../config.ini"))
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

/**
 * @brief Checks if a component is valid by verifying its existence in the component map.
 *
 * This function checks whether a component, identified by its name, is registered
 * in the component map. It returns `true` if the component is found, indicating
 * that it is a valid component. Otherwise, it returns `false`.
 *
 * @param component The name of the component to be checked.
 *
 * @return `true` if the component exists in the component map, otherwise `false`.
 */
bool isValidComponent(const std::string& component) {
    return componentMap.find(component) != componentMap.end();
}

void rtype::RType::createComponentViaConfig(GameLoader &gameLoader, size_t rtype, ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
    for (const auto& entity : gameLoader.globalConfig.items()) {
        const size_t id = entityManager.createEntity();
        for (const auto& comp : entity.value().items()) {
            if (!isValidComponent(comp.key())) {
                spdlog::error("Invalid component: {} in {}", comp.key(), entity.key());
                continue;
            }
            try {
                auto val = comp.value();
                const auto&[creator, type, registerComponent] = componentMap.at(comp.key());
                auto instance = creator();
                instance->create(val);
                registerComponent(entityManager, componentManager, id, std::move(instance));
            } catch (const std::out_of_range &e) {
                spdlog::error("Component not found in map: {}", comp.key());
            } catch (const std::bad_any_cast &e) {
                spdlog::error("Failed to cast component: {}", comp.key());
            } catch (const std::exception &e) {
                spdlog::error("Exception while creating component {}: {}", comp.key(), e.what());
            }
        }
    }
}

int rtype::RType::_run() {
    ecs::EntityManager entityManager;
    ecs::ComponentManager componentManager;
    ecs::SystemManager systemManager(entityManager, componentManager);

    GameLoader gameLoader;
    gameLoader.loadGlobalConfig("../config/config.json");
    const size_t rtype = entityManager.createEntity();
    this->createComponentViaConfig(gameLoader, rtype, entityManager, componentManager);

    GameLoader levelLoader;
    levelLoader.loadGlobalConfig("../config/levels/level_1.json");
    const size_t level = entityManager.createEntity();
    this->createComponentViaConfig(levelLoader, level, entityManager, componentManager);


  // TODO: use mode manager and make good exceptions
    network::TCPNetwork tcpNetwork(_port);
    network::UDPNetwork udpNetwork(_port);

    try {
        tcpNetwork.start();
        udpNetwork.start();
    } catch (std::exception &e) {
        spdlog::error("Error while starting tcp network: {}", e.what());
        throw;
    }

    systemManager.addSystem(rtype::systems::Movement::move);
    systemManager.addSystem(rtype::systems::Movement::startMove);

#ifdef RTYPE_IS_CLIENT
    systemManager.addSystem(rtype::systems::RenderWindowSys::createWindow);
    systemManager.addSystem(rtype::systems::RenderWindowSys::render);
#endif

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
