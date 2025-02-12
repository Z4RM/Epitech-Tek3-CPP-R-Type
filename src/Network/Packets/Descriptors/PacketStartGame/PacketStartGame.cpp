/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet start game
*/

#include "PacketStartGame.hpp"

#include <spdlog/spdlog.h>

namespace rtype::network {
    std::vector<char> PacketStartGame::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->level));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &this->level, sizeof(this->level));

        return buffer;
    }

    void PacketStartGame::fillData(const std::vector<char> &buffer) {
        std::memcpy(&this->level, buffer.data() + sizeof(this->_code), sizeof(this->level));
    }


}
