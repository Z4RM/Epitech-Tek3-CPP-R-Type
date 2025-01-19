/*
** EPITECH PROJECT, 2025
** RType
** File description:
** player shoot
*/

#pragma once
#include "Network/Packets/APacket.hpp"
namespace rtype::network {
    class PacketPlayerShoot : public APacket {
    public:
        ~PacketPlayerShoot() override = default;
        explicit PacketPlayerShoot(const int id = 0) : APacket(EPacketCode::PLAYER_SHOOT), netId(id) {};

        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        int netId = 0;
    };
}