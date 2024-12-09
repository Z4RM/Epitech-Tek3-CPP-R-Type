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

/**
 * @class EntityManager
 * @brief Manages the creation, destruction, and state of entities in an Entity-Component-System (ECS).
 *
 * The `EntityManager` class provides functionality for creating unique entity IDs,
 * destroying entities, and checking the active state of entities. Recycled IDs are reused
 * for better memory and ID management.
 */
namespace rtype::ecs
{
    struct Entity
    {
        size_t id;
    };

    class EntityManager {
    public:
        /**
         * @brief Creates a new entity.
         *
         * If there are recycled IDs available, the oldest ID is reused. Otherwise, a new
         * unique ID is generated. The created entity is added to the set of active entities.
         *
         * @return The unique ID of the newly created entity.
         */
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

        /**
         * @brief Destroys an entity.
         *
         * Marks the entity as inactive by removing it from the set of active entities
         * and adds its ID to the queue of available IDs for future reuse.
         *
         * @param entity The unique ID of the entity to destroy.
         */
        void destroyEntity(unsigned int entity) {
            if (_activeEntities.find(entity) != _activeEntities.end()) {
                _activeEntities.erase(entity);
                _availableIds.push(entity);
            }
        }

        /**
         * @brief Checks if an entity is active.
         *
         * Determines whether the specified entity is currently active in the system.
         *
         * @param entity The unique ID of the entity to check.
         * @return `true` if the entity is active, otherwise `false`.
         */
        bool isEntityActive(unsigned int entity) const {
            return _activeEntities.find(entity) != _activeEntities.end();
        }

        /**
         * @brief Retrieves the set of active entities.
         *
         * This method provides access to the set of active entities currently managed
         * by the ECS framework. Each entity is represented by an unsigned integer.
         *
         * @return A constant reference to an `std::unordered_set` containing the IDs
         *         of all active entities.
        */
        std::unordered_set<unsigned int> getEntities() const {
            return _activeEntities;
        }

    private:
        /**
         * @brief The next unique ID to assign to a new entity.
         *
         * If no recycled IDs are available, this value is incremented and used as the next ID.
         */
        unsigned int _nextId = 0;

        /**
         * @brief A set of IDs representing active entities.
         *
         * This set keeps track of all entities currently active in the system.
         */
        std::unordered_set<unsigned int> _activeEntities;

        /**
         * @brief A queue of recycled entity IDs.
         *
         * Contains IDs of destroyed entities that can be reused for new entities.
         */
        std::queue<unsigned int> _availableIds;
    };
}

#endif /* !ENTITY_MANAGER_HPP_ */
