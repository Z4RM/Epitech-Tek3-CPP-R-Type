#include "Movement.hpp"
#include "Components.hpp"
#include <complex>

#include "RType/ModeManager/ModeManager.hpp"

float rtype::systems::Movement::getDistanceBetweenPositions(const rtype::components::Position *pos1,
                                                            const rtype::components::Position *pos2) {
    return std::sqrt(
            std::pow(pos2->x - pos1->x, 2) +
            std::pow(pos2->y - pos1->y, 2) +
            std::pow(pos2->z - pos1->z, 2)
    );
}

bool rtype::systems::Movement::isColliding(components::Position *pos, components::Hitbox *hitBox,
                                           components::Position *colliderPos, components::Hitbox *colliderHitBox) {
    const auto distanceToCollider = getDistanceBetweenPositions(pos, colliderPos);
    const auto combinedWidth = (hitBox->size.width + colliderHitBox->size.width) / 2.0f;
    const auto combinedHeight = (hitBox->size.height + colliderHitBox->size.height) / 2.0f;
    return distanceToCollider <= combinedWidth || distanceToCollider <= combinedHeight;
}

void rtype::systems::Movement::handleCollisions(unsigned int entity, components::Position *pos, components::Hitbox *hitBox,
                                                const std::unordered_set<unsigned int> &entities,
                                                ecs::ComponentManager &componentManager, components::Velocity *vel) {
    constexpr float bounceFactor = 1.0f;  // Intensity of the bounce
    constexpr float minSeparation = 0.01f; // Small offset to avoid overlap

    for (auto &collisionEntity: entities) {
        if (collisionEntity == entity)
            continue;

        const auto colliderPos = componentManager.getComponent<components::Position>(collisionEntity);
        const auto colliderHitBox = componentManager.getComponent<components::Hitbox>(collisionEntity);
        const auto entityHealthBar = componentManager.getComponent<components::Health>(entity);
        const auto colliderDamage = componentManager.getComponent<components::Damage>(collisionEntity);

        if (!colliderPos || !colliderHitBox)
            continue;

        if (isColliding(pos, hitBox, colliderPos, colliderHitBox)) {
            float dx = pos->x - colliderPos->x;
            float dy = pos->y - colliderPos->y;
            float dz = pos->z - colliderPos->z;
            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);


            if (entityHealthBar && colliderDamage) {
                entityHealthBar->takeDamage(colliderDamage->collisionDamage);
            }

            if (distance > 0.0f) {
                dx /= distance;
                dy /= distance;
                dz /= distance;

                pos->x += dx * minSeparation;
                pos->y += dy * minSeparation;
                pos->z += dz * minSeparation;

                float dotProduct = vel->x * dx + vel->y * dy + vel->z * dz;

                vel->x -= (1.0f + bounceFactor) * dotProduct * dx;
                vel->y -= (1.0f + bounceFactor) * dotProduct * dy;
                vel->z -= (1.0f + bounceFactor) * dotProduct * dz;
            }
        }
    }
}

void rtype::systems::Movement::move(const rtype::ecs::EntityManager& entityManager,
                                    rtype::ecs::ComponentManager& componentManager) {
    static auto lastUpdateTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    const auto entities = entityManager.getEntities();

    for (auto &entity : entities) {
        const auto pos = componentManager.getComponent<components::Position>(entity);
        const auto vel = componentManager.getComponent<components::Velocity>(entity);
        const auto hitBox = componentManager.getComponent<components::Hitbox>(entity);
        const auto speed = componentManager.getComponent<components::Speed>(entity);
        const auto health = componentManager.getComponent<components::Health>(entity);

        if (pos && vel) {
            if (hitBox) {
                handleCollisions(entity, pos, hitBox, entities, componentManager, vel);
            }

            auto newPosX = vel->x * elapsedTime.count();
            auto newPosY = vel->y * elapsedTime.count();
            auto newPosZ = vel->z * elapsedTime.count();

            if (speed) {
                    newPosX *= speed->value;
                    newPosY *= speed->value;
                    newPosZ *= speed->value;
            }

            pos->x += newPosX;
            pos->y += newPosY;
            pos->z += newPosZ;

        #ifndef RTYPE_IS_SERVER
            if (health && hitBox) {
                health->bgBar.setPosition({pos->x + hitBox->size.width / 5, pos->y});
                health->healthBar.setPosition({pos->x + hitBox->size.width / 5, pos->y});
            }
        #endif

        }

        const auto ia = componentManager.getComponent<components::IA>(entity);
        const auto pos2 = componentManager.getComponent<components::Position>(entity);
        if (ia && pos2) {
            const auto move = ia->moves.begin();
            components::Velocity velTarget = move->second;

            if (hitBox) {
                handleCollisions(entity, pos2, hitBox, entities, componentManager, &velTarget);
            }

            auto newPosX = velTarget.x * elapsedTime.count();
            auto newPosY = velTarget.y * elapsedTime.count();
            auto newPosZ = velTarget.z * elapsedTime.count();

            if (speed) {
                newPosX *= speed->value;
                newPosY *= speed->value;
                newPosZ *= speed->value;
            }
            pos2->x += newPosX;
            pos2->y += newPosY;
            pos2->z += newPosZ;

        #ifndef RTYPE_IS_SERVER
            if (health && hitBox) {
                health->bgBar.setPosition({pos2->x + hitBox->size.width / 5, pos2->y});
                health->healthBar.setPosition({pos2->x + hitBox->size.width / 5, pos2->y});
            }
        #endif

        }
    }
}
