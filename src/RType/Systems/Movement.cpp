/*
** EPITECH PROJECT, 2025
** RType
** File description:
** InputSystem.cpp
*/

#include <complex>
#include "RType/ModeManager/ModeManager.hpp"
#include "Components.hpp"
#include "Movement.hpp"

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
                                                ecs::ComponentManager &componentManager,
                                                components::Velocity *vel,
                                                ecs::EntityManager &entityManager) {
    for (auto &collisionEntity: entities) {
        if (collisionEntity == entity)
            continue;

        const auto colliderPos = componentManager.getComponent<components::Position>(collisionEntity);
        const auto colliderHitBox = componentManager.getComponent<components::Hitbox>(collisionEntity);
        const auto entityHealthBar = componentManager.getComponent<components::Health>(entity);
        const auto colliderDamage = componentManager.getComponent<components::Damage>(collisionEntity);
        const auto peaceful = componentManager.getComponent<components::NoDamageToPlayer>(collisionEntity);
        const auto player = componentManager.getComponent<components::NetworkConnection>(entity);
        const auto ai1 = componentManager.getComponent<components::IA>(entity);
        const auto ai2 = componentManager.getComponent<components::IA>(collisionEntity);

        if (ai1 && ai2)
            continue;

        if (!colliderPos || !colliderHitBox)
            continue;

        if (isColliding(pos, hitBox, colliderPos.get(), colliderHitBox.get())) {
            if (entityHealthBar && colliderDamage && entityHealthBar->value > 0) {
                if (player && peaceful) {
                    continue;
                }
                auto now = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed = now - entityHealthBar->_elapsedDamage;
                if (elapsed.count() > 0.8 || (peaceful && ai1)) {
                    entityHealthBar->takeDamage(colliderDamage->collisionDamage);
                    entityHealthBar->_elapsedDamage = now;
                    componentManager.addComponent<components::Health>(entity, *entityHealthBar);
                    if (entityHealthBar->value <= 0) {
                        if (!player)
                            entityManager.destroyEntity(entity, componentManager);
                    }
                    if (peaceful && ai1) {
                        entityManager.destroyEntity(collisionEntity, componentManager);
                    }
                }
            }
        }
    }
}

void rtype::systems::Movement::move(rtype::ecs::EntityManager& entityManager,
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
        const auto peaceful = componentManager.getComponent<components::NoDamageToPlayer>(entity);

        if (pos && vel) {
            if (hitBox) {
                handleCollisions(entity, pos.get(), hitBox.get(), entities, componentManager, vel.get(), entityManager);
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

            if (peaceful) {
                if (pos->x > 800 || pos->x < 0 || pos->y < 0 || pos->y > 600) {
                    entityManager.destroyEntity(entity, componentManager);
                    continue;
                }
            }

            if (pos->x < 800 && pos->x > 0 && pos->y > 0 && pos->y < 600) {
                componentManager.addComponent<components::Position>(entity, *pos);
            }

        #ifdef RTYPE_IS_CLIENT
            if (health && hitBox && !peaceful) {
                health->bgBar.setPosition({pos->x + hitBox->size.width / 5, pos->y});
                health->healthBar.setPosition({pos->x + hitBox->size.width / 5, pos->y});
                componentManager.addComponent<components::Health>(entity, *health);
            }
        #endif

        }

        const auto ia = componentManager.getComponent<components::IA>(entity);
        const auto pos2 = componentManager.getComponent<components::Position>(entity);

        if (ia && pos2 && !peaceful) {
            const auto move = ia->moves.begin();
            components::Velocity velTarget = move->second;

            if (hitBox) {
                handleCollisions(entity, pos2.get(), hitBox.get(), entities, componentManager, &velTarget, entityManager);
            }

            auto newPosX = velTarget.x * elapsedTime.count() * 1.2;
            auto newPosY = velTarget.y * elapsedTime.count() * 1.2;
            auto newPosZ = velTarget.z * elapsedTime.count() * 1.2;

            if (speed) {
                newPosX *= speed->value;
                newPosY *= speed->value;
                newPosZ *= speed->value;
            }
            pos2->x += newPosX;
            pos2->y += newPosY;
            pos2->z += newPosZ;

            if (IS_SERVER && (pos2->x > 900 || pos2->x < -50 || pos2->y < -50 || pos2->y > 800)) {
                entityManager.destroyEntity(entity, componentManager);
                return;
            }

            componentManager.addComponent<components::Position>(entity, *pos2);
        #ifdef RTYPE_IS_CLIENT
            if (health && hitBox) {
                health->bgBar.setPosition({pos2->x + hitBox->size.width / 5, pos2->y});
                health->healthBar.setPosition({pos2->x + hitBox->size.width / 5, pos2->y});
                componentManager.addComponent<components::Health>(entity, *health);
            }
        #endif

        }
    }
}
