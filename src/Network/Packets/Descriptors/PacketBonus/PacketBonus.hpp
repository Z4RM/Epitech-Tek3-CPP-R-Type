/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet bonus
*/

#pragma once

#include <RType/Components/Shared/Position.hpp>

#include "Network/Packets/APacket.hpp"

namespace rtype::network {
    class PacketBonus : public APacket {
    public:
        ~PacketBonus() override = default;
        explicit PacketBonus(const int id = 0, components::Position pos = {0, 0, 0}, long eventId = 0, int netIdEmitter = -1) : APacket
        (EPacketCode::BONUS_SPAWN),
        type(id), pos(pos), eventId(eventId), netIdEmitter(netIdEmitter)
         {};

        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        int type = 0;
        long eventId = 0;
        components::Position pos;
        int netIdEmitter = -1;
    };
}