/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

// TODO: this is a sample code, remove it

#include "spdlog/spdlog.h"
#include "ECS.hpp"
#include "Components/Player.hpp"

#include <iostream>

int main() {
    rtype::ecs::EntityManager entityManager;
    rtype::ecs::ComponentManager componentManager;
    rtype::ecs::SystemManager systemManager;

    #ifdef RTYPE_IS_CLIENT
        spdlog::info("R-Type client launched");
    #endif
    #ifdef RTYPE_IS_SERVER
        spdlog::info("R-Type server launched on port 4242");
    #endif

    spdlog::info("R-Type launched");
    return 0;
}
