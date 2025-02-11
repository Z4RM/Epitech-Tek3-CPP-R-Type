/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet playershoot
*/

#include "PacketPlayerShoot.hpp"
namespace rtype::network {
    std::vector<char> PacketPlayerShoot::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->netId) + sizeof(this->isSuperProjectile) + sizeof
        (this->eventId) + sizeof(pos.x) + sizeof(pos.y));

        size_t currentSize = 0;

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        currentSize += sizeof(this->_code);
        std::memcpy(buffer.data() + currentSize, &this->netId, sizeof(this->netId));
        currentSize += sizeof(this->netId);
        std::memcpy(buffer.data() + currentSize, &this->isSuperProjectile, sizeof(this->isSuperProjectile));
        currentSize+= sizeof(this->isSuperProjectile);
        std::memcpy(buffer.data() + currentSize, &this->eventId, sizeof(this->eventId));
        currentSize += sizeof(this->eventId);
        std::memcpy(buffer.data() + currentSize, &this->pos.x, sizeof(this->pos.x));
        currentSize += sizeof(this->pos.x);
        std::memcpy(buffer.data() + currentSize, &this->pos.y, sizeof(this->pos.y));
        return buffer;
    }

    void PacketPlayerShoot::fillData(const std::vector<char> &buffer) {
        size_t currentSize = sizeof(this->_code);
        std::memcpy(&this->netId, buffer.data() + currentSize, sizeof(this->netId));
        currentSize += sizeof(this->netId);
        std::memcpy(&this->isSuperProjectile, buffer.data() + currentSize, sizeof(this->isSuperProjectile));
        currentSize += sizeof(this->isSuperProjectile);
        std::memcpy(&this->eventId, buffer.data() + currentSize, sizeof(this->eventId));
        currentSize += sizeof(this->eventId);
        std::memcpy(&this->pos.x, buffer.data() + currentSize, sizeof(this->pos.x));
        currentSize += sizeof(this->pos.x);
        std::memcpy(&this->pos.y, buffer.data() + currentSize, sizeof(this->pos.y));
    }
}