/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include "../APacket.hpp"

namespace rtype::network {
  class PacketConnect final : public APacket {
    public:
      ~PacketConnect() override = default;
      PacketConnect() : APacket(EPacketCode::CONNECT) {};
      [[nodiscard]] std::vector<char> bufferize() const override;
  };
}