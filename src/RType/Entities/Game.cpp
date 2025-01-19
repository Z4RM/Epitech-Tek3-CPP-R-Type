/*
** EPITECH PROJECT, 2025
** RType
** File description:
** game entity
*/

#include "Game.hpp"

#include "RType/Components/Shared/GameState.hpp"

namespace rtype::entities {
    Game::Game(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager) : AEntity(entityManager) {
        components::GameState state;

        state.isStarted = false;

        componentManager.addComponent(_id, state);
    }

}
