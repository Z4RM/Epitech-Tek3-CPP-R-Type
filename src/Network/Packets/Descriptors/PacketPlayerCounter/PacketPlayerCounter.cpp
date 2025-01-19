/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet playerCounter
*/

#include "PacketPlayerCounter.hpp"

#include <spdlog/spdlog.h>

namespace rtype::network {
    std::vector<char> PacketPlayerCounter::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->_count));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));

        std::memcpy(buffer.data() + sizeof(this->_code), &this->_count, sizeof(this->_count));
        return buffer;
    }

    void PacketPlayerCounter::fillData(const std::vector<char> &buffer) {
        std::memcpy(&_count, buffer.data() + sizeof(this->_code), sizeof(_count));
    }
}
