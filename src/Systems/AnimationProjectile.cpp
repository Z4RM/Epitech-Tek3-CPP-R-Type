/*
** EPITECH PROJECT, 2025
** RType
** File description:
** AnimationProjectile
*/

#include "AnimationProjectile.hpp"

void rtype::systems::UpdateProjectilesSystem::updateProjectiles(
    ecs::EntityManager &entityManager,
    ecs::ComponentManager &componentManager
) {
    // Correctif
    std::vector<size_t> projectileIds;

    for (auto& entity : entityManager.getEntities()) {
        if (componentManager.getComponent<Projectile>(entity)) {
            projectileIds.push_back(entity);
        }
    }

    auto it = projectileIds.begin();
    while (it != projectileIds.end()) {
        auto *projectile = componentManager.getComponent<Projectile>(*it);
        if (!projectile) {
            it = projectileIds.erase(it);
            continue;
        }
        projectile->position.x += projectile->velocity.x;
        projectile->position.y += projectile->velocity.y;

        if (projectile->position.x < 0 || projectile->position.x > 800 ||
            projectile->position.y < 0 || projectile->position.y > 600) {
            it = projectileIds.erase(it);
            } else {
                ++it;
            }

#ifdef RTYPE_IS_CLIENT
        float elapsedTime = projectile->animationClock.getElapsedTime().asSeconds();
        int frame = static_cast<int>(elapsedTime * projectile->animation.frameRate) % projectile->animation.nbFrames;

        int frameWidth = static_cast<int>(projectile->sprite.size.width);
        sf::IntRect textureRect(frame * frameWidth, 0, frameWidth, static_cast<int>(projectile->sprite.size.height));
        projectile->sprite.sprite->setTextureRect(textureRect);

        projectile->sprite.sprite->setPosition({projectile->position.x, projectile->position.y});
#endif
    }
}
