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

class ComponentManager {
public:
    template <typename T>
    void addComponent(unsigned int entity, const T& component) {
        if (component_sets.find(typeid(T)) == component_sets.end()) {
            component_sets[typeid(T)] = std::make_shared<SparseSet<T>>();
        }
        auto sparse_set = std::static_pointer_cast<SparseSet<T>>(component_sets[typeid(T)]);
        sparse_set->addComponent(entity, component);
    }

    template <typename T>
    void removeComponent(unsigned int entity) {
        if (component_sets.find(typeid(T)) != component_sets.end()) {
            auto sparse_set = std::static_pointer_cast<SparseSet<T>>(component_sets[typeid(T)]);
            sparse_set->removeComponent(entity);
        }
    }

    template <typename T>
    T* getComponent(unsigned int entity) {
        if (component_sets.find(typeid(T)) != component_sets.end()) {
            auto sparse_set = std::static_pointer_cast<SparseSet<T>>(component_sets[typeid(T)]);
            return sparse_set->getComponent(entity);
        }
        return nullptr;
    }
private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> component_sets;
};


#endif //COMPONENTMANAGER_HPP
