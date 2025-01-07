/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet factory
*/

#include "PacketFactory.hpp"

#include <Network/Packets/EPacketCode.hpp>
#include <spdlog/spdlog.h>

#include "Network/Packets/Descriptors/PacketConnect/PacketConnect.hpp"

namespace rtype::network {
    std::unique_ptr<IPacket> PacketFactory::fromCode(int code) {
        if (code == EPacketCode::CONNECT)
            return std::make_unique<PacketConnect>();
        throw PacketFactoryException();
    }

    std::unique_ptr<IPacket> PacketFactory::fromBuffer(const std::vector<char> &buffer) {
        int code = 0;
        std::memcpy(&code, buffer.data(), sizeof(int));
        if (code == CONNECT) {
            std::unique_ptr<PacketConnect> packet = std::make_unique<PacketConnect>();
            packet->fillData(buffer);
            return packet;
        }
        throw PacketFactoryException();
    }
}
