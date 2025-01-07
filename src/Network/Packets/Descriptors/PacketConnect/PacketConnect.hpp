/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include "../../APacket.hpp"

namespace rtype::network {
  /** @brief Represent a client packet to tell you try to connect as server **/
  class PacketConnect final : public APacket {
    public:
      ~PacketConnect() override = default;
      PacketConnect() : APacket(EPacketCode::CONNECT) {};
      [[nodiscard]] std::vector<char> bufferize() const override;
      void fillData(const std::vector<char> &buffer) override;
  };

}