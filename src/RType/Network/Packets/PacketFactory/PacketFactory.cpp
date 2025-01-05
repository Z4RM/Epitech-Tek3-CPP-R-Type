/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet factory
*/

#include "PacketFactory.hpp"

#include <RType/Network/Packets/EPacketCode.hpp>
#include <spdlog/spdlog.h>

#include "RType/Network/Packets/PacketConnect/PacketConnect.hpp"

namespace rtype::network {
    std::unique_ptr<IPacket> PacketFactory::fromCode(int code) {
        switch (code) {
            case EPacketCode::CONNECT:
                return std::make_unique<PacketConnect>();
            default: throw PacketFactoryException();
        }
    }

}
