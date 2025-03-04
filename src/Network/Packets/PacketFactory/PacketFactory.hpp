/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet factory
*/

#pragma once
#include <memory>

#include "Network/Packets/IPacket.hpp"
#include <stdexcept>

namespace rtype::network {
    /** @brief Creating packet **/
    class PacketFactory {
    public:
        /** @brief Exception in the packet factory **/
        class PacketFactoryException : public std::exception {
        public:
            PacketFactoryException() = default;
            [[nodiscard]] const char* what() const noexcept override {
                return "Invalid packet type";
            }
        };

        /** @brief creating the code packet class based on a received buffer **/
        static std::unique_ptr<IPacket> fromBuffer(const std::vector<char> &buffer);
    };
}
