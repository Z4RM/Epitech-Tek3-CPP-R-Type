/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Interface for scenes
*/

#pragma once
#include "ECS/SystemManager.hpp"

namespace rtype::ecs {
    class IScene {
      public:
        virtual void load();
        virtual void unload();
        virtual void update(SystemManager &sysMg);
        virtual bool isLoaded();
    };

}
