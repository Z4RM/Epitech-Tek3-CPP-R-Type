/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet factory
*/

#pragma once
#include <memory>

#include "RType/Network/Packets/IPacket.hpp"
#include <stdexcept>

namespace rtype::network {
    class PacketFactory {
    public:
        class PacketFactoryException : public std::exception {
        public:
            // TODO: documentation
            PacketFactoryException() = default;
            [[nodiscard]] const char* what() const noexcept override {
                return "Invalid packet type";
            }
        };

        static std::unique_ptr<IPacket> fromCode(int code);
    };
}
