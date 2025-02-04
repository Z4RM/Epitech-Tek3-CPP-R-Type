/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Enemy service
*/

#include "EnemyService.hpp"
#include "RType/Systems/Network/Network.hpp"

namespace rtype::services {
    void EnemyService::createEnemy(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
    components::Position pos, int netId) {
#ifdef RTYPE_IS_CLIENT
        components::Sprite sprite3 = {{600, 100, 0}, {33, 36}, "assets/sprites/enemy.gif", {1}};
        rtype::entities::Enemy enemy(
            entityManager,
            componentManager,
            pos,
            {0, 0, 0},
            {64, 64},
            sprite3,
            {"", 0, 0},
            { netId }
        );
#else
        systems::Network::globalNetId.store(systems::Network::globalNetId.load() + 1);

        rtype::entities::Enemy enemy(
        entityManager,
        componentManager,
        pos,
        {0, 0, 0},
        {64, 64},
        {systems::Network::globalNetId.load()}
        );

#endif
    }

}
