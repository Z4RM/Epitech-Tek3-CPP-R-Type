/*
** EPITECH PROJECT, 2025
** RType
** File description:
** AnimationProjectile
*/

#ifndef ANIMATIONPROJECTILE_HPP
#define ANIMATIONPROJECTILE_HPP

#include "../../include/ECS.hpp"
#include "../../include/Structures.hpp"

namespace rtype::systems {
    class UpdateProjectilesSystem {
    public:
        static void updateProjectiles(
        ecs::EntityManager &entityManager,
        ecs::ComponentManager &componentManager
        );
    };
}

#endif //ANIMATIONPROJECTILE_HPP
