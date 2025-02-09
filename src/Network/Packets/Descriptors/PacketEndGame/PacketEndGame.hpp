/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketEndGame.hpp
*/


#pragma once
#include "Network/Packets/APacket.hpp"

namespace rtype::network {
    class PacketEndGame : public APacket {
    public:
        ~PacketEndGame() override = default;
        explicit PacketEndGame(const bool isLose = false) : APacket(EPacketCode::END_GAME), isLose(isLose) {};

        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        bool isLose = false;
    };
}
