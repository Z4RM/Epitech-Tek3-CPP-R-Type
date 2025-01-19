/*
** EPITECH PROJECT, 2025
** RType
** File description:
** IPacket.hpp
*/

#pragma once
#include "asio.hpp"
#include "EPacketCode.hpp"

namespace rtype::network {
    /** @brief Interface for packets **/
    class IPacket {
    public:
        virtual ~IPacket() = default;
        //TODO: probably make directly a asio buffer here instead of manual array of bytes (see magic bytes and length in a array of const_buffer asio)
        /** @brief bufferize the packet to a vector of bytes **/
        [[nodiscard]] virtual std::vector<char> bufferize() const = 0;

        /** @brief returning the code of the packet **/
        [[nodiscard]] virtual EPacketCode getCode() const = 0;

        virtual void fillData(const std::vector<char> &buffer) = 0;
    };
}
