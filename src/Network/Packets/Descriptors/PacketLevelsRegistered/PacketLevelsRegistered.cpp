/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet levels registered
*/

#include "PacketLevelsRegistered.hpp"

namespace rtype::network {
    std::vector<char> PacketLevelsRegistered::bufferize() const {
        int dataCount = static_cast<int>(levels.size());

        std::vector<char> buffer(sizeof(this->_code) + sizeof(dataCount) + (sizeof(int) * dataCount));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &dataCount, sizeof(dataCount));

        size_t currentSize = sizeof(this->_code) + sizeof(dataCount);

        for (int level : levels) {
            std::memcpy(buffer.data() + currentSize, &level, sizeof(level));
            currentSize += sizeof(level);
        }
        return buffer;
    }

    void PacketLevelsRegistered::fillData(const std::vector<char> &buffer) {
        size_t currentSize = sizeof(this->_code) + sizeof(int);
        int dataCount = 0;
        std::memcpy(&dataCount, buffer.data() + sizeof(this->_code), sizeof(dataCount));

        levels.resize(dataCount);

        for (int i = 0; i < dataCount; i++) {
            std::memcpy(&this->levels[i], buffer.data() + currentSize, sizeof(this->levels[i]));
            currentSize += sizeof(this->levels[i]);
        }
    }
}