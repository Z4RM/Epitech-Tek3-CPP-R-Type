/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include <Network/Packets/APacket.hpp>

#include "RType/Models/EnemyData.hpp"

namespace rtype::network {

    class PacketEnemiesData final : public APacket {
    public:
        ~PacketEnemiesData() override = default;
        explicit PacketEnemiesData(std::vector<models::EnemyData> datas = {}) : APacket(EPacketCode::ENEMIES_DATA), datas(std::move
        (datas)) {};
        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        std::vector<models::EnemyData> datas {};
    };

}
