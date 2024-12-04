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

/**
 * @class SystemManager
 * @brief Manages and executes systems in an Entity-Component-System (ECS) architecture.
 *
 * The `SystemManager` class is responsible for storing and updating systems. Systems are
 * functions that define specific behaviors or operations to be applied in the ECS loop.
 */
namespace rtype::ecs
{
    class SystemManager {
    public:
        /**
         * @brief Adds a new system to the manager.
         *
         * Systems are stored as functions and executed in the order they are added.
         * A system is a function or callable object with no arguments and no return value.
         *
         * @param system The system function to add.
         */
        void addSystem(const std::function<void()>& system) {
            _systems.push_back(system);
        }

        /**
         * @brief Updates all registered systems.
         *
         * Executes each system function in the order they were added.
         */
        void updateSystems() {
            for (auto& system : _systems) {
                system();
            }
        }
    private:
        /**
         * @brief A list of registered systems.
         *
         * Stores all systems as callable objects (`std::function<void()>`) to be executed during updates.
         */
        std::vector<std::function<void()>> _systems;
    };
}

#endif //SYSTEMMANAGER_HPP
