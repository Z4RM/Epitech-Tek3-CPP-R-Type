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
            ecs::ComponentManager &componentManager,
            std::vector<size_t> &projectileIds
        );
    };
}

#endif //ANIMATIONPROJECTILE_HPP
