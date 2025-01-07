/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "Network/Packets/APacket.hpp"

namespace rtype::network {
    class PacketWelcome : public APacket {
    public:
        ~PacketWelcome() override = default;
        explicit PacketWelcome() : APacket(EPacketCode::WELCOME) {};
        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;
    };
}
