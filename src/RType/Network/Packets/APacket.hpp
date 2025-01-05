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
    class APacket : public virtual IPacket {
    protected:
         explicit APacket(const EPacketCode code) : _code(code) {};
        [[nodiscard]] EPacketCode getCode() const override {return _code;};
        const EPacketCode _code;
    };
}
