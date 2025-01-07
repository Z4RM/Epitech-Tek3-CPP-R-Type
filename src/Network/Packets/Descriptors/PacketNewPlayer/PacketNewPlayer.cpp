/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#include "PacketNewPlayer.hpp"

namespace rtype::network {
    std::vector<char> PacketNewPlayer::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        return buffer;
    }

    void PacketNewPlayer::fillData(const std::vector<char> &buffer) {

    }


}