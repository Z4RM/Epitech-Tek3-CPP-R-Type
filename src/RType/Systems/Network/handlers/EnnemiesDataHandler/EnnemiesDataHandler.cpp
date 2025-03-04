/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Ennemies Data handler
*/

#include "EnnemiesDataHandler.hpp"
#include "Components.hpp"
#include "Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.hpp"
#include <cmath>
#include <spdlog/spdlog.h>

#include "RType/Entities/Enemy.hpp"
#include "RType/Services/EnemyService/EnemyService.hpp"

namespace rtype::systems {
    void EnnemiesDataHandler::handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) {
        auto* enemiesData = dynamic_cast<network::PacketEnemiesData*>(packet.get());

        if (enemiesData) {
            for (const models::EnemyData &data: enemiesData->datas) {
                bool created = false;

                for (const auto &entity : _entityManager.getEntities()) {
                    auto net = _componentManager.getComponent<components::NetId>(entity);
                    auto ai = _componentManager.getComponent<components::IA>(entity);

                    if (net && ai) {
                        int netId = net->id;

                        if (data.netId.id == netId) {
                            created = true;
                            const auto localPos = _componentManager.getComponent<components::Position>(entity);
                            const auto vel = _componentManager.getComponent<components::Velocity>(entity);
                            const auto health = _componentManager.getComponent<components::Health>(entity);

                            if (!localPos || !vel || !health)
                                continue;

                            if (data.health != health->value) {
                                health->value = data.health;
                                health->takeDamage(0);
                                _componentManager.addComponent<components::Health>(entity, *health, _entityManager);
                            }

                            double distance = std::sqrt(
                                    std::pow(data.pos.x - localPos->x, 2) +
                                    std::pow(data.pos.y - localPos->y, 2) +
                                    std::pow(data.pos.z - localPos->z, 2)
                                );
                            const float positionThreshold = 0.1f;

                            if (distance > positionThreshold) {
                                *localPos = data.pos;
                                _componentManager.addComponent<components::Position>(entity, *localPos, _entityManager);
                            }
                            *vel = data.vel;
                            _componentManager.addComponent<components::Velocity>(entity, *vel, _entityManager);
                            break;
                        }
                    }
                }

                if (!created) {
                    spdlog::debug("Creating new enemy in the game with netId: {} with type : {}", data.netId.id,
                    std::to_string(data.type));
                    services::EnemyService::createEnemy(_entityManager, _componentManager, data.pos, data.netId.id, data.type);
                }
            }

            for (const auto entity : _entityManager.getEntities()) {
                auto net = _componentManager.getComponent<components::NetId>(entity);
                auto ai = _componentManager.getComponent<components::IA>(entity);

                bool isDead = true;

                if (net && net->id != 0 && ai) {
                    for (const models::EnemyData &data : enemiesData->datas) {
                        if (data.netId.id == net->id)
                            isDead = false;
                    }
                    if (isDead) {
                        spdlog::debug("Destroying disconnected enemy");
                        _entityManager.destroyEntity(entity);
                        _componentManager.removeAllComponent(entity);
                    }
                }
            }
        } else {
            spdlog::error("Invalid player data packet received");
        }
    }

}
