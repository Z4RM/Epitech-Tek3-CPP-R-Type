/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet playerCounter
*/

#include "PacketPlayerCounter.hpp"

namespace rtype::network {
    std::vector<char> PacketPlayerCounter::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->count));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));

        std::memcpy(buffer.data() + sizeof(this->_code), &this->count, sizeof(this->count));
        return buffer;
    }

    void PacketPlayerCounter::fillData(const std::vector<char> &buffer) {
        std::memcpy(&count, buffer.data() + sizeof(this->_code), sizeof(count));
    }
}
