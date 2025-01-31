/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet player counter
*/

#pragma once
#include <Network/Packets/APacket.hpp>

namespace rtype::network {
    class PacketPlayerCounter final : public APacket {
    public:
        ~PacketPlayerCounter() override = default;
        explicit PacketPlayerCounter(int count = 0) : APacket(EPacketCode::PLAYER_COUNT), count(count) {};
        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        int count = 0;
    };
}