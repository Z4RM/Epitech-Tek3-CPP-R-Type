/*
** EPITECH PROJECT, 2025
** RType
** File description:
** player shoot
*/

#pragma once
#include <RType/Components/Shared/Position.hpp>

#include "Network/Packets/APacket.hpp"
namespace rtype::network {
    class PacketPlayerShoot : public APacket {
    public:
        ~PacketPlayerShoot() override = default;
        explicit PacketPlayerShoot(const int id = 0, const bool isSuperProjectile = false, int eventId = 0,
        components::Position pos = {0, 0, 0}, bool isPlayer = true) : APacket(EPacketCode::PLAYER_SHOOT), netId(id), isSuperProjectile(isSuperProjectile), eventId(eventId), pos(pos), isPlayer(isPlayer) {};

        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        int netId = 0;
        bool isSuperProjectile = false;
        int eventId = 0;
        components::Position pos {};

        bool isPlayer = true;
    };
}