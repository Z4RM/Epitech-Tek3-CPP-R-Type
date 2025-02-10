/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketEndGame.cpp
*/


#include "PacketEndGame.hpp"

namespace rtype::network {
    std::vector<char> PacketEndGame::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->isLose));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &this->isLose, sizeof(this->isLose));

        return buffer;
    }

    void PacketEndGame::fillData(const std::vector<char> &buffer) {
        std::memcpy(&this->isLose, buffer.data() + sizeof(this->_code), sizeof(this->isLose));
    }
} // rtype