/*
** EPITECH PROJECT, 2025
** RType
** File description:
** GameState
*/

#pragma once
#include <map>

#include "RType/Models/Scene.hpp"

namespace rtype::components {
    struct GameState {
        std::map<int, models::Scene> _scenes;
        int currentScene;
    };
}
