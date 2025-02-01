/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet playershoot
*/

#include "PacketPlayerShoot.hpp"
namespace rtype::network {
    std::vector<char> PacketPlayerShoot::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->netId) + sizeof(this->isSuperProjectile));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &this->netId, sizeof(this->netId));
        std::memcpy(buffer.data() + sizeof(this->_code) + sizeof(this->netId), &this->isSuperProjectile, sizeof(this->isSuperProjectile));

        return buffer;
    }

    void PacketPlayerShoot::fillData(const std::vector<char> &buffer) {
        std::memcpy(&this->netId, buffer.data() + sizeof(this->netId), sizeof(this->netId));
        std::memcpy(&this->isSuperProjectile, buffer.data() + sizeof(this->_code) + sizeof(this->netId), sizeof(this->isSuperProjectile));
    }
}