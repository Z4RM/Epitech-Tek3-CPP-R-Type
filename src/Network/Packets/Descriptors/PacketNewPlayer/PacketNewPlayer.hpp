/*
** EPITECH PROJECT, 2025
** RType
** File description:
**
*/

#pragma once
#include <Network/Packets/APacket.hpp>

namespace rtype::network {

    class PacketNewPlayer final : public APacket {
    public:
        ~PacketNewPlayer() override = default;
        PacketNewPlayer() : APacket(EPacketCode::NEW_PLAYER) {};
        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;
    };
}
