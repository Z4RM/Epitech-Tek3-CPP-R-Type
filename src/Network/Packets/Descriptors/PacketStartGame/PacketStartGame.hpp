/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet start game
*/

#pragma once
#include <Network/Packets/APacket.hpp>

namespace rtype::network {
    class PacketStartGame final : public APacket {
    public:
        ~PacketStartGame() override = default;
        explicit PacketStartGame(int level = 0) : APacket(EPacketCode::START_GAME), level(level) {};
        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        int level = 0;
    };

}
