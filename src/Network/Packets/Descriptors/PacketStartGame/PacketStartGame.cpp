/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet start game
*/

#include "PacketStartGame.hpp"

namespace rtype::network {
    std::vector<char> PacketStartGame::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));

        return buffer;
    }

    void PacketStartGame::fillData(const std::vector<char> &buffer) {

    }


}