/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketPlayersData.hpp
*/

#pragma once
#include <Network/Packets/APacket.hpp>
#include <utility>

#include "RType/Models/PlayerData.hpp"

namespace rtype::network {

    class PacketPlayersData final : public APacket {
    public:
        ~PacketPlayersData() override = default;
        explicit PacketPlayersData(std::vector<models::PlayerData> datas = {}) : APacket(EPacketCode::PLAYERS_DATA), datas(std::move
        (datas)) {};
        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        std::vector<models::PlayerData> datas {};
    };

}
