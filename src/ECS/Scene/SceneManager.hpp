/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Scene manager
*/

#pragma once

#include <map>
#include <utility>
#include <spdlog/spdlog.h>

#include "IScene.hpp"
#include "ECS/SystemManager.hpp"
#include "RType/Models/Scene.hpp"

namespace rtype::ecs {
    class SceneManager {
    public:

       static SceneManager &getInstance() {
           static SceneManager instance;

           return instance;
       };

       void registerScene(int id, std::shared_ptr<IScene> scene) {
           std::lock_guard guard(_mutex);
           if (_scenes.find(id) != _scenes.end()) {
               spdlog::warn("Scene not loaded because already registered with ID {}", id);
               return;
           }
           _scenes[id] = std::move(scene);
       }

        void changeScene(const int id, bool unload = false) {
           std::lock_guard guard(_mutex);
            if (unload)
                this->_scenes[_currentScene]->unload();
            this->_currentScene = id;
        };

        void updateCurrentScene(SystemManager &sysMg) {
            std::unique_lock lock(_mutex);
            std::shared_ptr<IScene> scene = this->_scenes[this->_currentScene];

            if (!scene->isLoaded())
                scene->load();
            lock.unlock();
            scene->update(sysMg);
       }

    private:
        SceneManager() = default;
        std::mutex _mutex;
        int _currentScene = 0;
        std::map<int, std::shared_ptr<IScene>> _scenes {};
    };
}
