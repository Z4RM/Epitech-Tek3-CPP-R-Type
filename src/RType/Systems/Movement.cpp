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

#include "RType/Components/Shared/ChildEntities.hpp"
#include "RType/Components/Shared/ParentEntity.hpp"

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
        const auto player = componentManager.getComponent<components::NetworkConnection>(entity);
        const auto actualPlayer = componentManager.getComponent<components::ActualPlayer>(entity);
        const auto ai1 = componentManager.getComponent<components::IA>(entity);
        const auto ai2 = componentManager.getComponent<components::IA>(collisionEntity);
        auto bonus = componentManager.getComponent<components::Bonus>(collisionEntity);
        auto playerBonuses = componentManager.getComponent<components::PlayerBonuses>(entity);
        auto childEntities = componentManager.getComponent<components::ChildEntities>(entity);
        auto parentEntity = componentManager.getComponent<components::ParentEntity>(entity);
        auto projectileInfo = componentManager.getComponent<components::ProjectileInfo>(collisionEntity);
        bool childEntitiesAlive = false;

        if (childEntities) {
            for (auto &entity: childEntities->childEntities) {
                auto childHealth = componentManager.getComponent<components::Health>(entity);
                if (childHealth->value > 0) {
                    childEntitiesAlive = true;
                    break;
                }
            }
        }

        if (childEntitiesAlive) {
            continue;
        }
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
#endif
            if (entityHealthBar && colliderDamage && entityHealthBar->value > 0) {
                if (player && projectileInfo && projectileInfo->isPlayer)
                    continue;
                if (projectileInfo && ai1 && projectileInfo->isPlayer == false)
                    continue;
                auto now = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed = now - entityHealthBar->_elapsedDamage;
                if (elapsed.count() > 0.8 || (projectileInfo && projectileInfo->isPlayer && ai1)) {
                    bool isDodging = false;
#ifdef RTYPE_IS_SERVER
                    if (playerBonuses) {
                        for (auto bonuses : playerBonuses->bonuses) {
                            if (bonuses == models::SHIELD) {
                                int nb = dist(gen);
                                spdlog::warn(nb);
                                if (nb <= 30) //30% dodge
                                    isDodging = true;
                            }
                        }
                    }
#endif
                    if (isDodging) {
                        entityHealthBar->_elapsedDamage = now;
                        componentManager.addComponent<components::Health>(entity, *entityHealthBar, entityManager);
                        continue;
                    }
                    entityHealthBar->takeDamage(colliderDamage->collisionDamage);
                    entityHealthBar->_elapsedDamage = now;
                    componentManager.addComponent<components::Health>(entity, *entityHealthBar, entityManager);
                    if (entityHealthBar->value <= 0) {
                        if (!player && !parentEntity) {
                            entityManager.destroyEntity(entity);
                            componentManager.removeAllComponent(entity);
                            if (childEntities) {
                                for (auto &childEntity : childEntities->childEntities) {
                                    entityManager.destroyEntity(childEntity);
                                    componentManager.removeAllComponent(childEntity);
                                }
                            }
                        }
                        if (playerBonuses) {
                            componentManager.removeComponent<components::PlayerBonuses>(entity);
                        }
                    }
                    if (projectileInfo && ai1) {
                        if (!projectileInfo->isSuperProjectile) {
                            entityManager.destroyEntity(collisionEntity);
                            componentManager.removeAllComponent(collisionEntity);
                        }
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
    float deltaTime = elapsedTime.count();
    lastUpdateTime = currentTime;
    std::random_device rd;
    std::mt19937 gen(rd());

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

            auto newPosX = vel->x * deltaTime;
            auto newPosY = vel->y * deltaTime;
            auto newPosZ = vel->z * deltaTime;

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
            const auto parentEntity = componentManager.getComponent<components::ParentEntity>(entity);

            if (parentEntity && ia && pos) {
                auto parentPos = componentManager.getComponent<components::Position>(parentEntity->entity);

                if (parentPos) {
                    pos->y = parentPos->y + parentEntity->yOffset;
                }
            }

            if (ia && pos && !peaceful) {
                if (!ia->moves.empty()) {
                    ia->timeUntilNextMove -= deltaTime;

                    if ( ia->timeUntilNextMove <= 0.0f) {
                        std::uniform_int_distribution<size_t> dist(0, ia->moves.size() - 1);
                        int newMoveIndex = dist(gen);

                        if (ia->lastMoveIndex != -1) {
                            std::unordered_set<int> invalidMoves;
                            invalidMoves.insert(ia->lastMoveIndex);

                            std::vector<int> validMoves;
                            for (size_t i = 0; i < ia->moves.size(); ++i) {
                                if (invalidMoves.find(i) == invalidMoves.end()) {
                                    validMoves.push_back(i);
                                }
                            }

                            if (!validMoves.empty()) {
                                std::uniform_int_distribution<size_t> dist(0, validMoves.size() - 1);
                                newMoveIndex = validMoves[dist(gen)];
                            }
                        }

                        ia->lastMoveIndex = newMoveIndex;
                        std::uniform_real_distribution<float> timeDist(ia->minMoveTime, ia->maxMoveTime);
                        ia->timeUntilNextMove = timeDist(gen);
                    }
                    componentManager.addComponent(entity, *ia, entityManager);
                    components::Velocity velTarget = ia->moves[ia->lastMoveIndex];

                    if (hitBox) {
                        handleCollisions(entity, pos.get(), hitBox.get(), entities, componentManager, &velTarget, entityManager);
                    }

                    auto newPosX = velTarget.x * deltaTime * 1.2;
                    auto newPosY = velTarget.y * deltaTime * 1.2;
                    auto newPosZ = velTarget.z * deltaTime * 1.2;

                    if (speed) {
                        newPosX *= speed->value;
                        newPosY *= speed->value;
                        newPosZ *= speed->value;
                    }
                    pos->x += newPosX;
                    pos->y += newPosY;
                    pos->z += newPosZ;

                    if (IS_SERVER && (pos->x > 900 || pos->x < -50)) {
                        entityManager.destroyEntity(entity);
                        componentManager.removeAllComponent(entity);
                        return;
                    }
                }

                if (pos->y > 10 && pos->y < 560 && !parentEntity) {
                    if (ia->type == models::BOSS_CASK || ia->type == models::BOSS_ALIEN || ia->type == models::BOSS_TURRET ||
                    ia->type == models::BOSS_STAROS || ia->type == models::BOSS_HEART || ia->type == models::BOSS_ROBOT || ia
                    ->type == models::BOSS_MACHIN || ia->type == models::BOSS_DARKPLAYER) {
                        if (pos->x > 60 && pos->x < 800)
                            componentManager.addComponent<components::Position>(entity, *pos, entityManager);
                    } else
                        componentManager.addComponent<components::Position>(entity, *pos, entityManager);
                } else if (parentEntity) {
                    componentManager.addComponent<components::Position>(entity, *pos, entityManager);
                }
            }
#ifdef RTYPE_IS_CLIENT
            if (hitBox) {
                hitBox->rect.setPosition({pos->x, pos->y});
                componentManager.addComponent<components::Hitbox>(entity, *hitBox, entityManager);
            }
            if (health && pos) {
                health->bgBar.setPosition({pos->x - 25, pos->y - 20});
                health->healthBar.setPosition({pos->x - 25, pos->y - 20});
                componentManager.addComponent<components::Health>(entity, *health, entityManager);
            }
#endif
        }
    }
}
