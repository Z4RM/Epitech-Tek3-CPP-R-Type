/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** SparseSet.hpp
*/

#ifndef SPARSESET_HPP
#define SPARSESET_HPP

#include <vector>
#include <unordered_map>

template <typename T>
class SparseSet {
public:
    void addComponent(unsigned int entity, const T& component) {
        if (_sparse.find(entity) == _sparse.end()) {
            _sparse[entity] = _dense.size();
            _dense.push_back(entity);
            _components.push_back(component);
        }
    }

    void removeComponent(unsigned int entity) {
        if (_sparse.find(entity) != _sparse.end()) {
            size_t index = _sparse[entity];
            unsigned int last_entity = _dense.back();

            _dense[index] = last_entity;
            _sparse[last_entity] = index;

            _dense.pop_back();
            _components[index] = _components.back();
            _components.pop_back();

            _sparse.erase(entity);
        }
    }

    std::unique_ptr<T> getComponent(unsigned int entity) {
        if (_sparse.find(entity) != _sparse.end()) {
            return std::make_unique<T>(_components[_sparse[entity]]);
        }
        return nullptr;
    }

    const std::vector<unsigned int>& getEntities() const {
        return _dense;
    }

private:
    std::unordered_map<unsigned int, size_t> _sparse;
    std::vector<unsigned int> _dense;
    std::vector<T> _components;
};

#endif //SPARSESET_HPP
