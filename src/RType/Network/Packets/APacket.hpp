/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Represent the basic packet whose packet derive from
*/

#pragma once
#include "./EPacketCode.hpp"
#include "IPacket.hpp"

namespace rtype::network {
    class APacket : public IPacket {
    protected:
        explicit APacket(const EPacketCode code) : code(code) {};
    public:
        const EPacketCode code;
    };
}
