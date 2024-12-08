/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** ComponentManager.hpp
*/

#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "SparseSet.hpp"

/**
 * @class ComponentManager
 * @brief Manages components associated with entities in an Entity-Component-System (ECS).
 *
 * The `ComponentManager` class allows adding, removing, and retrieving generic type components
 * associated with entities identified by unique IDs. Components are stored in specialized sparse sets (`SparseSet`) for each type.
 */
namespace rtype::ecs
{
    class ComponentManager {
    public:
        /**
         * @brief Adds a component to an entity.
         *
         * If the component type does not yet exist, a new sparse set (`SparseSet`) is created for that type.
         * The component is then added to the specified entity.
         *
         * @tparam T The type of the component.
         * @param entity The unique identifier of the entity.
         * @param component The component to add.
         */
        template <typename T>
        void addComponent(unsigned int entity, const T& component) {
            if (_componentSets.find(typeid(T)) == _componentSets.end()) {
                _componentSets[typeid(T)] = std::make_shared<SparseSet<T>>();
            }
            auto sparse_set = std::static_pointer_cast<SparseSet<T>>(_componentSets[typeid(T)]);
            sparse_set->addComponent(entity, component);
        }

        /**
         * @brief Removes a component from an entity.
         *
         * If a sparse set for the component type exists, the component associated with the specified
         * entity is removed.
         *
         * @tparam T The type of the component.
         * @param entity The unique identifier of the entity.
         */
        template <typename T>
        void removeComponent(unsigned int entity) {
            if (_componentSets.find(typeid(T)) != _componentSets.end()) {
                auto sparse_set = std::static_pointer_cast<SparseSet<T>>(_componentSets[typeid(T)]);
                sparse_set->removeComponent(entity);
            }
        }

        /**
         * @brief Retrieves a component associated with an entity.
         *
         * If a sparse set for the component type exists and the entity has a component of that type,
         * it is returned. Otherwise, returns `nullptr`.
         *
         * @tparam T The type of the component.
         * @param entity The unique identifier of the entity.
         * @return A pointer to the component, or `nullptr` if the entity does not have a component of this type.
         */
        template <typename T>
        T* getComponent(unsigned int entity) {
            if (_componentSets.find(typeid(T)) != _componentSets.end()) {
                auto sparse_set = std::static_pointer_cast<SparseSet<T>>(_componentSets[typeid(T)]);
                return sparse_set->getComponent(entity);
            }
            return nullptr;
        }

    private:
        /**
         * @brief A mapping of component types to sparse sets (`SparseSet`).
         *
         * The keys in the map are type indices (`std::type_index`), and the values are
         * shared pointers (`std::shared_ptr<void>`) to the corresponding sparse sets.
         */
        std::unordered_map<std::type_index, std::shared_ptr<void>> _componentSets;
    };
}


#endif //COMPONENTMANAGER_HPP
