/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "asio.hpp"
#include "EPacketCode.hpp"

namespace rtype::network {
    class IPacket {
    public:
        virtual ~IPacket() = default;
        //TODO: probably make directly a asio buffer here instead of manual array of bytes (see magic bytes and length in a array of const_buffer asio)
        [[nodiscard]] virtual std::vector<char> bufferize() const = 0;
        [[nodiscard]] virtual EPacketCode getCode() const = 0;
    };
}
