/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** EntityManager.hpp
*/

#ifndef ENTITY_MANAGER_HPP_
#define ENTITY_MANAGER_HPP_
#include <unordered_set>
#include <queue>

class EntityManager {
public:
    unsigned int createEntity() {
        unsigned int entity;
        if (!_availableIds.empty()) {
            entity = _availableIds.front();
            _availableIds.pop();
        } else {
            entity = _nextId++;
        }
        _activeEntities.insert(entity);
        return entity;
    }

    void destroyEntity(unsigned int entity) {
        if (_activeEntities.find(entity) != _activeEntities.end()) {
            _activeEntities.erase(entity);
            _availableIds.push(entity);
        }
    }

    bool isEntityActive(unsigned int entity) const {
        return _activeEntities.find(entity) != _activeEntities.end();
    }
private:
    unsigned int _nextId = 0;
    std::unordered_set<unsigned int> _activeEntities;
    std::queue<unsigned int> _availableIds;
};

#endif /* !ENTITY_MANAGER_HPP_ */
