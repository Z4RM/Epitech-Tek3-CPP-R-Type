/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#include "PacketWelcome.hpp"

#include <spdlog/spdlog.h>

namespace rtype::network {
    std::vector<char> PacketWelcome::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));

        return buffer;
    }

    void PacketWelcome::fillData(const std::vector<char> &buffer) {
    }
}
