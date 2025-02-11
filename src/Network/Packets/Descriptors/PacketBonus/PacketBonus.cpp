/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Packet bonus
*/

#include "PacketBonus.hpp"

namespace rtype::network {
    std::vector<char> PacketBonus::bufferize() const {
        std::vector<char> buffer(sizeof(this->_code) + sizeof(this->type) + sizeof(this->pos.x) + sizeof(this->pos.y) + sizeof
        (this->eventId) + sizeof(this->netIdEmitter));

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &this->type, sizeof(this->type));
        std::memcpy(buffer.data() + sizeof(this->_code) + sizeof(this->type), &this->pos.x, sizeof(this->pos.x));
        std::memcpy(buffer.data() + sizeof(this->_code) + sizeof(this->type) + sizeof(this->pos.x), &this->pos.y, sizeof(this->pos.y));
        std::memcpy(buffer.data() + sizeof(this->_code) + sizeof(this->type) + sizeof(this->pos.x) + sizeof(this->pos.y),
        &this->eventId,sizeof(this->eventId));
        std::memcpy(buffer.data() + sizeof(this->_code) + sizeof(this->type) + sizeof(this->pos.x) + sizeof(this->pos.y) +
        sizeof(this->eventId), &this->netIdEmitter,sizeof(this->netIdEmitter));
        return buffer;
    }

    void PacketBonus::fillData(const std::vector<char> &buffer) {
        this->pos.z = 0;
        std::memcpy(&this->type, buffer.data() + sizeof(this->_code), sizeof(this->type));
        std::memcpy(&this->pos.x, buffer.data() + sizeof(this->_code) + sizeof(this->type), sizeof(this->pos.x));
        std::memcpy(&this->pos.y, buffer.data() + sizeof(this->_code) + sizeof(this->type) + sizeof(this->pos.x), sizeof(this->pos.y));
        std::memcpy(&this->eventId, buffer.data() + sizeof(this->_code) + sizeof(this->type) + sizeof(this->pos.x) + sizeof(this->pos.y), sizeof(this->eventId));
        std::memcpy(&this->netIdEmitter, buffer.data() + sizeof(this->_code) + sizeof(this->type) + sizeof(this->pos.x) + sizeof(this->pos.y) + sizeof(this->eventId), sizeof(this->netIdEmitter));
    }
}