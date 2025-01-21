/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketWelcome.cpp
*/

#include "PacketWelcome.hpp"

namespace rtype::network {
    std::vector<char> PacketWelcome::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->netId));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &this->netId, sizeof(this->netId));

        return buffer;
    }

    void PacketWelcome::fillData(const std::vector<char> &buffer) {
        std::memcpy(&this->netId, buffer.data() + sizeof(this->netId), sizeof(this->netId));
    }
}
