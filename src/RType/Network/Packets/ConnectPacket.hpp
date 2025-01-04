/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include "./APacket.hpp"

namespace rtype::network {
  class ConnectPacket : public APacket {
    public:
      ConnectPacket() : APacket(EPacketCode::CONNECT) {};
  };
}