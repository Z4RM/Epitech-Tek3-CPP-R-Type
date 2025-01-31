/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PlayerDataHandler
*/

#include "PlayerDataHandler.hpp"
#include "Components.hpp"
#include "RType/ModeManager/ModeManager.hpp"
#include <spdlog/spdlog.h>

#include "Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp"
#include "RType/Services/PlayerService/PlayerService.hpp"

namespace rtype::systems {
    void PlayerDataHandler::handle(std::unique_ptr<network::IPacket> packet, asio::ip::udp::endpoint endpoint) {
        auto* playersData = dynamic_cast<network::PacketPlayersData*>(packet.get());

        if (playersData) {
            for (models::PlayerData &data : playersData->datas) {
                bool created = false;

                for (const auto &entity : _entityManager.getEntities()) {
                    auto netCo = _componentManager.getComponent<components::NetworkConnection>(entity);
                    auto pos = _componentManager.getComponent<components::Position>(entity);
                    auto vel = _componentManager.getComponent<components::Velocity>(entity);
                    auto size = _componentManager.getComponent<components::Size>(entity);
                    auto net = _componentManager.getComponent<components::NetId>(entity);
                    auto actualPlayer = _componentManager.getComponent<components::ActualPlayer>(entity);

                    if (IS_SERVER) {
                        if (netCo) {
                            if (net->id == data.netId.id) {
                                if (!netCo->endpoint.has_value()) {
                                    spdlog::info("New player in the game with network ID {}", net->id);
                                    _componentManager.addComponent<components::NetworkConnection>(entity, {netCo->socket, endpoint});
                                }
                                    //*pos = data.pos;
                                *vel = data.vel;
                                *size = data.size;
                                _componentManager.addComponent<components::Velocity>(entity, *vel);
                                _componentManager.addComponent<components::Size>(entity, *size);
                            }
                        }
                    } else {
                        if (actualPlayer) {
                            int netId = net->id;

                            if (data.netId.id == netId) {
                                created = true;
                                const auto localPos = _componentManager.getComponent<components::Position>(entity);
                                const auto vel = _componentManager.getComponent<components::Velocity>(entity);
                                const auto health = _componentManager.getComponent<components::Health>(entity);

                                if (health) {
                                    if (data.health != health->value) {
                                        health->setHealth(data.health);
                                        health->_elapsedDamage = std::chrono::steady_clock::now();
                                        if (data.health <= 0)
                                            _componentManager.addComponent<components::Dead>(entity, { true });
                                        _componentManager.addComponent<components::Health>(entity, *health);
                                    }
                                }

                                if (localPos) {
                                    float distance = std::sqrt(
                                    std::pow(data.pos.x - localPos->x, 2) +
                                    std::pow(data.pos.y - localPos->y, 2) +
                                    std::pow(data.pos.z - localPos->z, 2)
                                    );
                                    const float positionThreshold = 0.1f;
                                    if (distance > positionThreshold) {
                                        *localPos = data.pos;
                                        _componentManager.addComponent<components::Position>(entity, *localPos);
                                    }
                                }

                                if (!actualPlayer->value) {
                                    *vel = data.vel;
                                    _componentManager.addComponent<components::Velocity>(entity, *vel);
                                }
                                break;
                            }
                        }
                    }
                }
#ifdef RTYPE_IS_CLIENT
                if (!created) {
                    spdlog::debug("Creating new player in the game");
                    services::PlayerService::createPlayer(data.netId.id, _entityManager, _componentManager, false);
                }
#endif
            }

            if (!IS_SERVER) {
                for (const auto entity : _entityManager.getEntities()) {
                    auto net = _componentManager.getComponent<components::NetId>(entity);
                    auto actualPlayer = _componentManager.getComponent<components::ActualPlayer>(entity);
                    bool isDead = true;

                    if (net && actualPlayer) {
                        for (const models::PlayerData &data : playersData->datas) {
                            if (data.netId.id == net->id) {
                                isDead = false;
                            }
                        }
                        if (isDead) {
                            spdlog::debug("Destroying disconnected player");
                            _entityManager.destroyEntity(entity, _componentManager);
                        }
                    }
                }
            }
        } else {
            spdlog::error("Invalid player data packet received");
        }
    }
}
