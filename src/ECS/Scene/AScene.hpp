/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Abstract scene class
*/

#pragma once
#include "IScene.hpp"
#include "ECS/IEntity.hpp"

namespace rtype::ecs {
    class AScene : public IScene {
    protected:
        ~AScene() override = default;

        AScene(EntityManager &entityManager, ComponentManager &componentManager) : _entityManager(entityManager),
        _componentManager(componentManager) {};

        void update(SystemManager &sysMg) final { sysMg.updateSystems(_entityManager, _componentManager); }

        void unload() final {
            for (const auto &entity : _entities) {
                _entityManager.destroyEntity(entity, this->_componentManager);
            }
            this->_loaded = false;
        };

        void registerEntity(IEntity &entity) final { this->_entities.emplace_back(entity.getId()); }

        void registerEntity(unsigned int entity) final { this->_entities.emplace_back(entity); }

        bool isLoaded() final { return this->_loaded; }

        void load() override { this->_loaded = true; };

        EntityManager &_entityManager;
        ComponentManager &_componentManager;
        bool _loaded = false;
    private:
        std::vector<unsigned int> _entities;
    };
}
