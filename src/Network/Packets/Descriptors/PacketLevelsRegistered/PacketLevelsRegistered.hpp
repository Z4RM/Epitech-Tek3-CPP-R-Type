/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet levels registered
*/

#pragma once

#include "Network/Packets/APacket.hpp"
#include "RType/Levels/Level.hpp"

namespace rtype::network {
    /** @brief Packet used to tell what levels are registered in the server to the client **/
    class PacketLevelsRegistered final : public APacket {
    public:
        ~PacketLevelsRegistered() override = default;
        explicit PacketLevelsRegistered(const std::vector<int> &levels) : APacket(EPacketCode::LEVELS_REGISTERED),
        levels(levels) {};
        PacketLevelsRegistered() : APacket(EPacketCode::LEVELS_REGISTERED) {};

        [[nodiscard]] std::vector<char> bufferize() const override;
        void fillData(const std::vector<char> &buffer) override;

        std::vector<int> levels {}; ///< registered levels
    };
}
