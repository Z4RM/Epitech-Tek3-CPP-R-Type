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

#include <spdlog/spdlog.h>

#include "RType/Components/Shared/Bonus.hpp"
#include "RType/Components/Shared/PlayerBonuses.hpp"
#include "RType/Components/Shared/ProjectileInfo.hpp"
#include <random>

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
    float halfWidthA = hitBox->size.width / 2.0f;
    float halfHeightA = hitBox->size.height / 2.0f;
    float halfWidthB = colliderHitBox->size.width / 2.0f;
    float halfHeightB = colliderHitBox->size.height / 2.0f;

    float leftA = pos->x - halfWidthA;
    float rightA = pos->x + halfWidthA;
    float topA = pos->y - halfHeightA;
    float bottomA = pos->y + halfHeightA;

    float leftB = colliderPos->x - halfWidthB;
    float rightB = colliderPos->x + halfWidthB;
    float topB = colliderPos->y - halfHeightB;
    float bottomB = colliderPos->y + halfHeightB;

    bool xOverlap = (leftA < rightB) && (rightA > leftB);
    bool yOverlap = (topA < bottomB) && (bottomA > topB);

    return xOverlap && yOverlap;
}

void rtype::systems::Movement::handleCollisions(unsigned int entity, components::Position *pos, components::Hitbox *hitBox,
                                                const std::unordered_set<unsigned int> &entities,
                                                ecs::ComponentManager &componentManager,
                                                components::Velocity *vel,
                                                ecs::EntityManager &entityManager) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    for (auto &collisionEntity: entities) {
        if (collisionEntity == entity)
            continue;

        const auto colliderPos = componentManager.getComponent<components::Position>(collisionEntity);
        const auto colliderHitBox = componentManager.getComponent<components::Hitbox>(collisionEntity);
        const auto entityHealthBar = componentManager.getComponent<components::Health>(entity);
        const auto colliderDamage = componentManager.getComponent<components::Damage>(collisionEntity);
        const auto peaceful = componentManager.getComponent<components::NoDamageToPlayer>(collisionEntity);
        const auto player = componentManager.getComponent<components::NetworkConnection>(entity);
        const auto actualPlayer = componentManager.getComponent<components::ActualPlayer>(entity);
        const auto ai1 = componentManager.getComponent<components::IA>(entity);
        const auto ai2 = componentManager.getComponent<components::IA>(collisionEntity);
        auto bonus = componentManager.getComponent<components::Bonus>(collisionEntity);
        auto playerBonuses = componentManager.getComponent<components::PlayerBonuses>(entity);

        if (ai1 && ai2)
            continue;

        if (!colliderPos || !colliderHitBox)
            continue;

        if (isColliding(pos, hitBox, colliderPos.get(), colliderHitBox.get())) {
            if (actualPlayer && bonus) {
                entityManager.destroyEntity(collisionEntity);
                componentManager.removeAllComponent(collisionEntity);
                continue;
            }
#ifdef RTYPE_IS_SERVER
            if (player && bonus) {
                components::PlayerBonuses newBonuses = {};
                if (playerBonuses) {
                    newBonuses = *playerBonuses;
                }
                newBonuses.bonuses.emplace_back(bonus->type);
                componentManager.addComponent<components::PlayerBonuses>(entity, newBonuses, entityManager);
                entityManager.destroyEntity(collisionEntity);
                componentManager.removeAllComponent(collisionEntity);
            }

            bool isDodging = false;
            if (playerBonuses) {
                for (auto bonuses : playerBonuses->bonuses) {
                    if (bonuses == models::FORCE) {
                        isDodging = (dist(gen) <= 30); // 30% dodge
                    }
                }
            }
            if (isDodging)
                continue;
#endif
            if (entityHealthBar && colliderDamage && entityHealthBar->value > 0) {
                if (player && peaceful) {
                    continue;
                }
                auto now = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed = now - entityHealthBar->_elapsedDamage;
                if (elapsed.count() > 0.8 || (peaceful && ai1)) {
                    entityHealthBar->takeDamage(colliderDamage->collisionDamage);
                    entityHealthBar->_elapsedDamage = now;
                    componentManager.addComponent<components::Health>(entity, *entityHealthBar, entityManager);
                    if (entityHealthBar->value <= 0) {
                        if (!player) {
                            entityManager.destroyEntity(entity);
                            componentManager.removeAllComponent(entity);
                        }
                    }
                    if (peaceful && ai1) {
                        entityManager.destroyEntity(collisionEntity);
                        componentManager.removeAllComponent(collisionEntity);
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
        const auto projectileInfo = componentManager.getComponent<components::ProjectileInfo>(entity);

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

            if (projectileInfo) {
                if (pos->x > 800 || pos->x < 0 || pos->y < 0 || pos->y > 600) {
                    entityManager.destroyEntity(entity);
                    componentManager.removeAllComponent(entity);
                    continue;
                }
                componentManager.addComponent<components::Position>(entity, *pos, entityManager);
            } else {
                if (pos->x < 780 && pos->x > 0 && pos->y > 0 && pos->y < 580) {
                    componentManager.addComponent<components::Position>(entity, *pos, entityManager);
                }
            }
#ifdef RTYPE_IS_CLIENT
            auto playerBonuses = componentManager.getComponent<components::PlayerBonuses>(entity);
            if (playerBonuses && pos) {
                for (auto &bonuses: playerBonuses->bonuses) {
                    bonuses.second.setPosition(pos->x + 32, pos->y);
                }
                componentManager.addComponent<components::PlayerBonuses>(entity, *playerBonuses, entityManager);
            }
#endif

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
                    entityManager.destroyEntity(entity);
                    componentManager.removeAllComponent(entity);
                    return;
                }

                componentManager.addComponent<components::Position>(entity, *pos2, entityManager);
            }
#ifdef RTYPE_IS_CLIENT
            if (hitBox) {
                hitBox->rect.setPosition({pos->x, pos->y});
                componentManager.addComponent<components::Hitbox>(entity, *hitBox, entityManager);
            }
            if (health && pos2) {
                health->bgBar.setPosition({pos2->x - 25, pos2->y - 20});
                health->healthBar.setPosition({pos2->x - 25, pos2->y - 20});
                componentManager.addComponent<components::Health>(entity, *health, entityManager);
            }
#endif
        }
    }
}
