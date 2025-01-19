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
#include <shared_mutex>

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

        void addComponent(unsigned int entity, const T& component) {
            std::unique_lock lock(_mutex);

            auto it = _sparse.find(entity);
            if (it != _sparse.end()) {
                size_t index = it->second;
                _components[index] = component;
            } else {
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
        void removeComponent(unsigned int entity) override {
            std::lock_guard lock(_mutex);

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

        std::shared_ptr<T> getComponent(unsigned int entity) {
            std::lock_guard lock(_mutex);
            auto sparseIt = _sparse.find(entity);
            if (sparseIt == _sparse.end()) {
                return nullptr;
            }
            size_t index = sparseIt->second;
            if (index >= _dense.size() || index >= _components.size()) {
                return nullptr;
            }
            return std::make_shared<T>(_components[index]);
        }

    private:
        std::unordered_map<unsigned int, size_t> _sparse;
        std::vector<unsigned int> _dense;
        std::vector<T> _components;
        mutable std::mutex _mutex;
    };
}

#endif //SPARSESET_HPP
