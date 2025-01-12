#include "Movement.hpp"
#include "Components.hpp"
#include <complex>

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

void
rtype::systems::Movement::handleCollisions(unsigned int entity, components::Position *pos, components::Hitbox *hitBox,
                                           const std::unordered_set<unsigned int> &entities,
                                           ecs::ComponentManager &componentManager, components::Velocity *vel) {
    std::vector<unsigned int> collidingEntities;
    for (auto &collisionEntity: entities) {
        if (collisionEntity == entity)
            continue;
        const auto colliderPos = componentManager.getComponent<components::Position>(collisionEntity);
        const auto colliderHitBox = componentManager.getComponent<components::Hitbox>(collisionEntity);
        if (!colliderPos || !colliderHitBox)
            continue;
        if (isColliding(pos, hitBox, colliderPos, colliderHitBox)) {
            float dx = pos->x - colliderPos->x;
            float dy = pos->y - colliderPos->y;
            float dz = pos->z - colliderPos->z;
            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (distance > 0.0f) {
                dx /= distance;
                dy /= distance;
                dz /= distance;
                float dotProduct = vel->x * dx + vel->y * dy + vel->z * dz;
                vel->x -= dotProduct * dx;
                vel->y -= dotProduct * dy;
                vel->z -= dotProduct * dz;
            }
        }
    }
}

void rtype::systems::Movement::move(const rtype::ecs::EntityManager& entityManager, rtype::ecs::ComponentManager& componentManager)
{
    const auto entities = entityManager.getEntities();

    for (auto &entity: entities) {
        const auto pos = componentManager.getComponent<components::Position>(entity);
        const auto vel = componentManager.getComponent<components::Velocity>(entity);
        const auto hitBox = componentManager.getComponent<components::Hitbox>(entity);

        if (pos && vel) {
            if (hitBox)
                handleCollisions(entity, pos, hitBox, entities, componentManager, vel);
            pos->x += vel->x;
            pos->y += vel->y;
            pos->z += vel->z;
        }
        const auto ia = componentManager.getComponent<components::IA>(entity);
        const auto pos2 = componentManager.getComponent<components::Position>(entity);
        if (ia && pos2) {
            const auto move = ia->moves.begin();
            const components::Velocity velTarget = move->second;
            pos2->x += velTarget.x;
            pos2->y += velTarget.y;
            pos2->z += velTarget.z;
        }
    }
}
