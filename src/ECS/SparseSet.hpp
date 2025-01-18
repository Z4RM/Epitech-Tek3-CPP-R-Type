/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** SparseSet.hpp
*/

#ifndef SPARSESET_HPP
#define SPARSESET_HPP
#include <iostream>

#include <vector>
#include <unordered_map>

/**
 * @class SparseSet
 * @brief Manages a sparse set for efficient storage and retrieval of components.
 *
 * The `SparseSet` class is designed to handle components associated with entities in an ECS (Entity-Component-System).
 * It provides efficient methods for adding, removing, and accessing components, while keeping track of associated entities.
 *
 * @tparam T The type of the components to store.
 */
namespace rtype::ecs
{
    class ISparseSet
    {
    public:
        virtual ~ISparseSet() = default;
        virtual void removeComponent(unsigned int entity) = 0;
    };

    template <typename T>
    class SparseSet : public ISparseSet {
    public:
        SparseSet() = default;
        ~SparseSet() override = default;
        /**
         * @brief Adds a component for a given entity.
         *
         * If the entity does not already have a component, it is added to the dense array, and its
         * index is stored in the sparse map. The associated component is stored in a separate dense array.
         *
         * @param entity The unique ID of the entity.
         * @param component The component to associate with the entity.
         */
        void addComponent(unsigned int entity, const T& component) {
            std::lock_guard lock(_mutex);
            if (_sparse.find(entity) == _sparse.end()) {
                _sparse[entity] = _dense.size();
                _dense.push_back(entity);
                _components.push_back(component);
            }
        }

        /**
         * @brief Removes the component associated with a given entity.
         *
         * If the entity has an associated component, it is removed. The dense array and
         * sparse map are updated to maintain efficiency.
         *
         * @param entity The unique ID of the entity.
         */
        void removeComponent(unsigned int entity) {
            std::lock_guard<std::mutex> lock(_mutex);

            auto it = _sparse.find(entity);
            if (it != _sparse.end()) {
                size_t index = it->second;

                unsigned int last_entity = _dense.back();
                _dense[index] = last_entity;
                _sparse[last_entity] = index;

                _dense.pop_back();
                _components[index] = _components.back();
                _components.pop_back();

                _sparse.erase(it);
            }
        }

        /**
         * @brief Retrieves the component associated with a given entity.
         *
         * If the entity has an associated component, a unique pointer to the component is returned.
         * Otherwise, returns `nullptr`.
         *
         * @param entity The unique ID of the entity.result = {std::unordered_map<unsigned int, unsigned long>} {2 elements}
         * @return A pointer to the component, or `nullptr` if the entity does not have a component.
         */
        T* getComponent(unsigned int entity) {
            std::lock_guard<std::mutex> lock(_mutex);

            try {
                if (_sparse.empty() || _sparse.count(entity) <= 0)
                    return nullptr;
                auto sparseIt = _sparse.find(entity);
                if (sparseIt != _sparse.end()) {
                    auto index = sparseIt->second;

                    if (index < _components.size()) {
                        return &_components[index];
                    }
                }
            } catch (std::exception &e) {
                return nullptr;
            }
            return nullptr;
        }

        /**
         * @brief Gets the list of entities currently stored in the set.
         *
         * Returns a reference to the dense array containing the IDs of all entities
         * with associated components.
         *
         * @return A reference to the vector of entity IDs.
         */
        const std::vector<unsigned int>& getEntities() const {
            return _dense;
        }

    private:
        /**
         * @brief A mapping from entity IDs to their indices in the dense array.
         */
        std::unordered_map<unsigned int, size_t> _sparse;

        /**
         * @brief A dense array of entity IDs.
         *
         * Contains the IDs of all entities with components, stored in a compact form
         * for efficient iteration.
         */
        std::vector<unsigned int> _dense;

        /**
         * @brief A dense array of components.
         *
         * Stores components in the same order as the corresponding entities in `_dense`.
         */
        std::vector<T> _components;

        mutable std::mutex _mutex;
    };
}

#endif //SPARSESET_HPP
