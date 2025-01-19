/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketWelcome.hpp
*/

#pragma once
#include "Network/Packets/APacket.hpp"

namespace rtype::network {
    class PacketWelcome : public APacket {
    public:
        ~PacketWelcome() override = default;
        explicit PacketWelcome(const int id = 0) : APacket(EPacketCode::WELCOME), netId(id) {};

        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        int netId = 0;
    };
}
