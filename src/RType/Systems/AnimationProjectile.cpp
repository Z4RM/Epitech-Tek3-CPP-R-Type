/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Anim projectile
*/

#include "AnimationProjectile.hpp"

void rtype::systems::UpdateProjectilesSystem::updateProjectiles(
    ecs::EntityManager &entityManager,
    ecs::ComponentManager &componentManager
) {
    std::vector<size_t> projectileIds;

    for (auto& entity : entityManager.getEntities()) {
        auto projectile = componentManager.getComponent<components::Projectile>(entity);
        auto sprite = componentManager.getComponent<components::Sprite>(entity);
        auto pos = componentManager.getComponent<components::Position>(entity);


        if (!projectile || !sprite || !pos)
            continue;

        if (pos->x < 0 || pos->x > 800 ||
        pos->y < 0 || pos->y > 600) {
            entityManager.destroyEntity(entity, componentManager);
            continue;
        }

        float elapsedTime = projectile->animationClock.getElapsedTime().asSeconds();
        int frame = static_cast<int>(elapsedTime * projectile->animation.frameRate) % projectile->animation.nbFrames;
        int frameWidth = static_cast<int>(sprite->size.width);
        sf::IntRect textureRect(frame * frameWidth + 82, 165, frameWidth, static_cast<int>(sprite->size.height));
        sprite->sprite->setTextureRect(textureRect);

        componentManager.addComponent<components::Sprite>(entity, *sprite);
    }
}