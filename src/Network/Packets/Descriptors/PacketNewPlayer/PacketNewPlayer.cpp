/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketNewPlayer.cpp
*/

#include "PacketNewPlayer.hpp"

namespace rtype::network {
    std::vector<char> PacketNewPlayer::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->id));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &this->id, sizeof(this->id));

        return buffer;
    }

    void PacketNewPlayer::fillData(const std::vector<char> &buffer) {
        std::memcpy(&this->id, buffer.data() + sizeof(this->_code), sizeof(this->id));
    }


}