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
    /** @brief Abstract class for packet's default implementations **/
    class APacket : public virtual IPacket {
    protected:
         explicit APacket(const EPacketCode code) : _code(code) {};

        /** @brief returning the packet code **/
        [[nodiscard]] EPacketCode getCode() const override { return _code; };

        /** @brief the packet code **/
        const EPacketCode _code;
    };
}
