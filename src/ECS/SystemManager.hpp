/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** SystemManager.hpp
*/

#ifndef SYSTEMMANAGER_HPP
#define SYSTEMMANAGER_HPP

#include <vector>
#include <functional>

class SystemManager {
public:
    void addSystem(const std::function<void()>& system) {
        _systems.push_back(system);
    }

    void updateSystems() {
        for (auto& system : _systems) {
            system();
        }
    }
private:
    std::vector<std::function<void()>> _systems;
};

#endif //SYSTEMMANAGER_HPP
