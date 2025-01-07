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
#include "Network/Packets/Descriptors/PacketNewPlayer/PacketNewPlayer.hpp"
#include "Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp"
#include "Network/Packets/Descriptors/PacketWelcome/PacketWelcome.hpp"

namespace rtype::network {
    std::unique_ptr<IPacket> PacketFactory::fromBuffer(const std::vector<char> &buffer) {
        int code = 0;
        std::memcpy(&code, buffer.data(), sizeof(int));
        std::unique_ptr<IPacket> packet;

        switch (code) {
            case CONNECT:
                packet = std::make_unique<PacketConnect>();
                break;
            case WELCOME:
                packet = std::make_unique<PacketWelcome>();
                break;
            case NEW_PLAYER:
                packet = std::make_unique<PacketNewPlayer>();
                break;
            case PLAYERS_DATA:
                packet = std::make_unique<PacketPlayersData>();
                break;
            default:
                throw PacketFactoryException();
        }
        packet->fillData(buffer);
        return packet;
    }

}
