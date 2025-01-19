/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Interface for scenes
*/

#pragma once
#include "ECS/SystemManager.hpp"
#include "ECS/IEntity.hpp"

namespace rtype::ecs {
    class IScene {
      public:
        virtual ~IScene() = default;
        virtual void load() = 0;
        virtual void unload() = 0;
        virtual void update(SystemManager &sysMg) = 0;
        virtual bool isLoaded() = 0;
        virtual void registerEntity(IEntity &entity) = 0;
    };

}
