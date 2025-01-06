/*
** EPITECH PROJECT, 2025
** RType
** File description:
**
*/

#include "./PacketConnect.hpp"

namespace rtype::network {
    std::vector<char> PacketConnect::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        return buffer;
    }
}